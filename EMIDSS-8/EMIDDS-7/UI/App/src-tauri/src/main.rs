// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use tauri::{AppHandle, Emitter};
use tokio::io::{AsyncBufReadExt, BufReader};
use tokio_serial::SerialPortBuilderExt;
use polars::prelude::*;
use serde_json::json;

// Command 1: Port Enumeration
#[tauri::command]
fn get_avaible_ports() -> Result<Vec<String>, String> {
    // Scan System for active ports
    let ports = serialport::available_ports().map_err(|e| e.to_string())?;

    // Extract the port names (ej: "COM2")
    let mut port_names = Vec::new();
    for port in ports {
        port_names.push(port.port_name);
    }

    Ok(port_names)
}

// Command 2: Async Serial Connection
#[tauri::command]
async fn connect_uart(app: AppHandle, port: String, baudrate: u32) -> Result<(), String> {
    // 1. Attempt to open the serial port
    let mut serial_stream = tokio_serial::new(&port, baudrate)
        .open_native_async()
        .map_err(|e| format!("Failed to open port {}: {}", port, e))?;

    // Required for Unix systems to prevent device lock errors
    #[cfg(unix)]
    serial_stream
        .set_exclusive(false)
        .map_err(|e| e.to_string())?;

    println!("Succesfully connected to {} at {} baud", port, baudrate);

    tauri::async_runtime::spawn(async move {
        let mut buf_reader = BufReader::new(serial_stream);
        let mut line_buf = String::new();
        let mut in_dump = false;
        let mut dump_lines = Vec::new();

        loop {
            line_buf.clear();
            // Wait asynchronously for new data to arrive on the hardware
            match buf_reader.read_line(&mut line_buf).await {
                Ok(0) => {
                    // EOF reached / port disconnected
                    println!("Serial port closed.");
                    break;
                }
                Ok(_) => {
                    let line = line_buf.trim().to_string();

                    // Implement a parser block to recognize bulk memory dump
                    if line == "BEGIN_DUMP" {
                        in_dump = true;
                        dump_lines.clear();
                        continue;
                    } else if line == "END_DUMP" {
                        in_dump = false;
                        // Feed parsed data into a Polars DataFrame and emit
                        if let Err(e) = process_and_emit_dump(&app, &dump_lines) {
                            eprintln!("Error processing dump: {}", e);
                        }
                        continue;
                    }

                    if in_dump {
                        dump_lines.push(line);
                    } else {
                        // Emit the string across the IPC bridge to the Web UI
                        if let Err(e) = app.emit("uart-rx", line_buf.clone()) {
                            eprintln!("Failed to emit to frontend: {}", e);
                        }
                    }
                }
                Err(e) => {
                    eprintln!("Error reading from serial port: {}", e);
                    break;
                }
            }
        }
    });

    Ok(())
}

// Polars parsing and interpolation
fn process_and_emit_dump(app: &AppHandle, lines: &[String]) -> Result<(), Box<dyn std::error::Error>> {
    let mut times = Vec::new();
    let mut temps: Vec<Option<f64>> = Vec::new();
    let mut hums: Vec<Option<f64>> = Vec::new();
    let mut press: Vec<Option<f64>> = Vec::new();

    for line in lines {
        let parts: Vec<&str> = line.split(',').collect();
        // Fallback for different expected formats
        if parts.len() >= 5 {
            let hour = parts[0].trim();
            let min = parts[1].trim();
            times.push(format!("{:0>2}:{:0>2}", hour, min));
            temps.push(parts[2].trim().parse::<f64>().ok());
            hums.push(parts[3].trim().parse::<f64>().ok());
            press.push(parts[4].trim().parse::<f64>().ok());
        } else if parts.len() == 4 {
            times.push(parts[0].trim().to_string());
            temps.push(parts[1].trim().parse::<f64>().ok());
            hums.push(parts[2].trim().parse::<f64>().ok());
            press.push(parts[3].trim().parse::<f64>().ok());
        }
    }

    let time_s = Series::new("Time".into(), times);
    let temp_s = Series::new("Temperature".into(), temps);
    let hum_s = Series::new("Humidity".into(), hums);
    let press_s = Series::new("Pressure".into(), press);

    let df = DataFrame::new(vec![time_s.into(), temp_s.into(), hum_s.into(), press_s.into()])?;
    
    // Interpolate missing values
    let lf = df.lazy().with_columns([
        col("Temperature").forward_fill(None),
        col("Humidity").forward_fill(None),
        col("Pressure").forward_fill(None),
    ]);
    
    let clean_df = lf.collect()?;

    let mut json_data = Vec::new();
    let times_col = clean_df.column("Time")?.str()?;
    let temps_col = clean_df.column("Temperature")?.f64()?;
    let hums_col = clean_df.column("Humidity")?.f64()?;
    let press_col = clean_df.column("Pressure")?.f64()?;

    for i in 0..clean_df.height() {
        let time_val = times_col.get(i).unwrap_or("");
        let temp_val = temps_col.get(i).unwrap_or(0.0);
        let hum_val = hums_col.get(i).unwrap_or(0.0);
        let press_val = press_col.get(i).unwrap_or(0.0);

        json_data.push(json!({
            "Time": time_val,
            "Temperature": temp_val,
            "Humidity": hum_val,
            "Pressure": press_val
        }));
    }

    app.emit("telemetry-update", json_data)?;

    Ok(())
}

// Main Builder
fn main() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        // Register custom commands so frontend can read it
        .invoke_handler(tauri::generate_handler![get_avaible_ports, connect_uart])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use tauri::{AppHandle, Emitter};
use tokio::io::AsyncReadExt;
use tokio_serial::SerialPortBuilderExt;

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

    // 2. Spawn a background "green thread" (tokio)
    tauri::async_runtime::spawn(async move {
        let mut buf = vec![0u8; 1024];

        loop {
            // Wait asynchronously for new data to arrive on the hardware
            match serial_stream.read(&mut buf).await {
                Ok(0) => {
                    // EOF reached / port disconnected
                    println!("Serial port closed.");
                    break;
                }
                Ok(n) => {
                    // Convert the raw bytes to a UTF-8 string
                    let payload = String::from_utf8_lossy(&buf[..n]).to_string();

                    // Emit the string across the IPC bridge to the Web UI
                    if let Err(e) = app.emit("uart-rx", payload) {
                        eprintln!("Failed to emit to frontend: {}", e);
                    }
                }
                Err(e) => {
                    eprintln!("Error readinf from serial port: {}", e);
                    break;
                }
            }
        }
    });

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

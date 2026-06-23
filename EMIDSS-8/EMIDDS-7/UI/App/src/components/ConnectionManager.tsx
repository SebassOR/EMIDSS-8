import { useEffect, useState } from "react";
import { invoke } from "@tauri-apps/api/core";

export function ConnectionManager() {
  const [ports, setPorts] = useState<string[]>([]);
  const [selectedPort, setSelectedPort] = useState("");
  const [baudrate, setBaudrate] = useState(115200);
  const [status, setStatus] = useState<
    "idle" | "loading" | "connected" | "error"
  >("idle");
  const [errorMessage, setErrorMessage] = useState("");

  async function loadPorts() {
    setStatus("loading");
    setErrorMessage("");

    try {
      let detectedPorts: string[];

      try {
        // Current backend command name
        detectedPorts = await invoke<string[]>("get_avaible_ports");
      } catch {
        // Fallback if the backend command name gets corrected later
        detectedPorts = await invoke<string[]>("get_available_ports");
      }

      setPorts(detectedPorts);

      // Select the first port by default
      if (detectedPorts.length > 0) {
        setSelectedPort(detectedPorts[0]);
      }

      setStatus("idle");
    } catch (error) {
      setStatus("error");
      setErrorMessage(String(error));
    }
  }

  async function handleConnect() {
    if (!selectedPort) {
      setStatus("error");
      setErrorMessage("No serial port selected.");
      return;
    }

    setStatus("loading");
    setErrorMessage("");

    try {
      // Rust handles the actual UART connection
      await invoke("connect_uart", {
        port: selectedPort,
        baudrate: Number(baudrate),
      });

      setStatus("connected");
    } catch (error) {
      setStatus("error");
      setErrorMessage(String(error));
    }
  }

  useEffect(() => {
    // Load ports when the component opens
    loadPorts();
  }, []);

  return (
    <section className="rounded-2xl border border-slate-700 bg-slate-900 p-5 shadow-lg">
      <div className="mb-4">
        <h2 className="text-lg font-semibold text-white">Connection Manager</h2>
        <p className="text-sm text-slate-400">
          Select the UART/COM port and baudrate for the EMIDSS-8 payload.
        </p>
      </div>

      <div className="grid gap-4 md:grid-cols-3">
        <label className="flex flex-col gap-2">
          <span className="text-sm font-medium text-slate-300">
            Serial Port
          </span>

          <select
            value={selectedPort}
            onChange={(event) => setSelectedPort(event.target.value)}
            className="rounded-lg border border-slate-700 bg-slate-950 px-3 py-2 text-slate-100 outline-none focus:border-blue-500"
          >
            {ports.length === 0 ? (
              <option value="">No ports detected</option>
            ) : (
              ports.map((port) => (
                <option key={port} value={port}>
                  {port}
                </option>
              ))
            )}
          </select>
        </label>

        <label className="flex flex-col gap-2">
          <span className="text-sm font-medium text-slate-300">Baudrate</span>

          <select
            value={baudrate}
            onChange={(event) => setBaudrate(Number(event.target.value))}
            className="rounded-lg border border-slate-700 bg-slate-950 px-3 py-2 text-slate-100 outline-none focus:border-blue-500"
          >
            <option value={9600}>9600</option>
            <option value={57600}>57600</option>
            <option value={115200}>115200</option>
          </select>
        </label>

        <div className="flex items-end gap-2">
          <button
            onClick={handleConnect}
            disabled={status === "loading" || !selectedPort}
            className="w-full rounded-lg bg-blue-600 px-4 py-2 font-medium text-white transition hover:bg-blue-500 disabled:cursor-not-allowed disabled:bg-slate-700"
          >
            {status === "loading" ? "Loading..." : "Connect"}
          </button>

          <button
            onClick={loadPorts}
            className="rounded-lg border border-slate-700 px-4 py-2 font-medium text-slate-200 transition hover:bg-slate-800"
          >
            Refresh
          </button>
        </div>
      </div>

      <div className="mt-4 flex items-center gap-2 text-sm">
        <span
          className={
            status === "connected"
              ? "h-2.5 w-2.5 rounded-full bg-green-400"
              : status === "error"
                ? "h-2.5 w-2.5 rounded-full bg-red-400"
                : "h-2.5 w-2.5 rounded-full bg-slate-500"
          }
        />

        <span className="text-slate-300">
          Status:{" "}
          <strong className="text-white">
            {status === "connected"
              ? "Connected"
              : status === "error"
                ? "Error"
                : status === "loading"
                  ? "Loading"
                  : "Idle"}
          </strong>
        </span>
      </div>

      {errorMessage && (
        <p className="mt-3 rounded-lg border border-red-800 bg-red-950/50 px-3 py-2 text-sm text-red-300">
          {errorMessage}
        </p>
      )}
    </section>
  );
}

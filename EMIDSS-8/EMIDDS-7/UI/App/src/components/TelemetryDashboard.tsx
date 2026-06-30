import { useEffect, useMemo, useState } from "react";
import { listen } from "@tauri-apps/api/event";
import { TelemetryLineChart } from "./TelemetryLineChart";

type TelemetryPoint = {
  Time: string;
  Temperature: number;
  Humidity: number;
  Pressure: number;
};

function normalizeTelemetryPayload(payload: TelemetryPoint[] | string): TelemetryPoint[] {
  const parsedPayload = typeof payload === "string" ? JSON.parse(payload) : payload;

  if (!Array.isArray(parsedPayload)) {
    throw new Error("Telemetry payload must be an array.");
  }

  return parsedPayload
    .map((point) => ({
      Time: String(point.Time),
      Temperature: Number(point.Temperature),
      Humidity: Number(point.Humidity),
      Pressure: Number(point.Pressure),
    }))
    .filter(
      (point) =>
        point.Time &&
        Number.isFinite(point.Temperature) &&
        Number.isFinite(point.Humidity) &&
        Number.isFinite(point.Pressure)
    );
}

export function TelemetryDashboard() {
  const [telemetryData, setTelemetryData] = useState<TelemetryPoint[]>([]);
  const [statusMessage, setStatusMessage] = useState("Waiting for telemetry-update data...");
  const [errorMessage, setErrorMessage] = useState("");

  // Development helper to validate chart rendering without hardware data
  function loadSampleTelemetry() {
  const sampleData: TelemetryPoint[] = [
    { Time: "00:00", Temperature: 24.5, Humidity: 45.2, Pressure: 101.3 },
    { Time: "00:01", Temperature: 24.8, Humidity: 45.5, Pressure: 101.2 },
    { Time: "00:02", Temperature: 25.1, Humidity: 46.0, Pressure: 101.1 },
    { Time: "00:03", Temperature: 25.4, Humidity: 46.4, Pressure: 101.0 },
    { Time: "00:04", Temperature: 25.8, Humidity: 46.8, Pressure: 100.9 },
  ];

  setTelemetryData(sampleData);
  setStatusMessage(`${sampleData.length} sample telemetry points loaded.`);
  setErrorMessage("");
}

  useEffect(() => {
    let unlisten: (() => void) | undefined;

    async function setupTelemetryListener() {
      // Listen for parsed telemetry data from Rust
      unlisten = await listen<TelemetryPoint[] | string>("telemetry-update", (event) => {
        try {
          const normalizedData = normalizeTelemetryPayload(event.payload);

          setTelemetryData(normalizedData);
          setStatusMessage(`${normalizedData.length} telemetry points loaded.`);
          setErrorMessage("");
        } catch (error) {
          setErrorMessage(String(error));
          setStatusMessage("Failed to parse telemetry payload.");
        }
      });
    }

    setupTelemetryListener();

    return () => {
      // Remove listener when the dashboard closes
      if (unlisten) {
        unlisten();
      }
    };
  }, []);

  const chartLabels = useMemo(
    () => telemetryData.map((point) => point.Time),
    [telemetryData]
  );

  const temperatureValues = useMemo(
    () => telemetryData.map((point) => point.Temperature),
    [telemetryData]
  );

  const humidityValues = useMemo(
    () => telemetryData.map((point) => point.Humidity),
    [telemetryData]
  );

  const pressureValues = useMemo(
    () => telemetryData.map((point) => point.Pressure),
    [telemetryData]
  );

  return (
    <section className="rounded-2xl border border-slate-700 bg-slate-900 p-5 shadow-lg">
      <div className="mb-5 flex flex-col gap-2 md:flex-row md:items-end md:justify-between">
        <div>
          <h2 className="text-lg font-semibold text-white">Telemetry Dashboard</h2>
          <p className="text-sm text-slate-400">
            Parsed memory dump data rendered from the backend telemetry-update event
          </p>
        </div>

        <div className="flex flex-col gap-2 md:items-end">
        <div className="rounded-lg border border-slate-800 bg-slate-950 px-3 py-2 text-sm text-slate-300">
            {statusMessage}
        </div>

        // Development helper to validate chart rendering without hardware data
        {import.meta.env.DEV && (
            <button
            onClick={loadSampleTelemetry}
            className="rounded-lg border border-slate-700 px-3 py-2 text-sm font-medium text-slate-200 transition hover:bg-slate-800"
            >
            Load Sample Data
            </button>
        )}
        </div>
      </div>

      {errorMessage && (
        <p className="mb-4 rounded-lg border border-red-800 bg-red-950/50 px-3 py-2 text-sm text-red-300">
          {errorMessage}
        </p>
      )}

      {telemetryData.length === 0 && (
        <div className="mb-4 rounded-lg border border-slate-800 bg-slate-950 px-4 py-3 text-sm text-slate-500">
          No telemetry data received yet. Use the Mission Control Panel to request a memory read
        </div>
      )}

      <div className="grid gap-4 xl:grid-cols-3">
        <TelemetryLineChart
          title="Temperature"
          labels={chartLabels}
          values={temperatureValues}
          unit="°C"
          borderColor="#38bdf8"
          backgroundColor="rgba(56, 189, 248, 0.15)"
        />

        <TelemetryLineChart
          title="Humidity"
          labels={chartLabels}
          values={humidityValues}
          unit="%"
          borderColor="#22c55e"
          backgroundColor="rgba(34, 197, 94, 0.15)"
        />

        <TelemetryLineChart
          title="Pressure"
          labels={chartLabels}
          values={pressureValues}
          borderColor="#f97316"
          backgroundColor="rgba(249, 115, 22, 0.15)"
        />
      </div>
    </section>
  );
}
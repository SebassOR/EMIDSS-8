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
  const [statusMessage, setStatusMessage] = useState("Awaiting payload memory transmission...");
  const [errorMessage, setErrorMessage] = useState("");

  useEffect(() => {
    let unlisten: (() => void) | undefined;

    async function setupTelemetryListener() {
      unlisten = await listen<TelemetryPoint[] | string>("telemetry-update", (event) => {
        try {
          const normalizedData = normalizeTelemetryPayload(event.payload);

          setTelemetryData(normalizedData);
          setStatusMessage(`${normalizedData.length} telemetry records loaded.`);
          setErrorMessage("");
        } catch (error) {
          setErrorMessage(String(error));
          setStatusMessage("Failed to parse incoming payload.");
        }
      });
    }

    setupTelemetryListener();

    return () => {
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
    <div className="flex flex-col">
      <div className="mb-6 flex flex-col gap-3 border-b border-[#262626] pb-4 sm:flex-row sm:items-center sm:justify-between">
        <div>
          <span className="text-xs font-semibold uppercase tracking-widest text-slate-400">
            ATMOSPHERIC TELEMETRY GRAPHS
          </span>
          <p className="text-xs text-slate-500 mt-1">
            Recorded sensor data across memory dumps.
          </p>
        </div>

        <div className="rounded border border-[#262626] bg-[#1a1a1a] px-3 py-1.5 font-mono text-xs text-slate-300">
          {statusMessage}
        </div>
      </div>

      {errorMessage && (
        <p className="mb-4 rounded border border-red-900/50 bg-red-950/30 px-3 py-2 font-mono text-xs text-red-400">
          {errorMessage}
        </p>
      )}

      {telemetryData.length === 0 && (
        <div className="mb-4 rounded border border-[#262626] bg-[#141414] px-4 py-3 font-mono text-xs text-slate-500">
          No telemetry records loaded yet. Use Operations Console to execute a memory read.
        </div>
      )}

      <div className="grid gap-6 xl:grid-cols-3">
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
    </div>
  );
}
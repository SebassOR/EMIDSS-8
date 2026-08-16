import type { TelemetryRecord } from "../types";

function Row({ label, value }: { label: string; value: string }) {
  return (
    <div className="flex items-baseline justify-between gap-3">
      <span className="text-[11px] font-medium uppercase tracking-wide text-zinc-500">{label}</span>
      <span className="text-sm font-semibold tabular-nums text-zinc-900 dark:text-zinc-50">{value}</span>
    </div>
  );
}

function fmt(value: number | undefined, digits: number, unit = ""): string {
  return value === undefined || value === null ? "—" : `${value.toFixed(digits)}${unit}`;
}

export function StatCards({ latest }: { latest: TelemetryRecord | undefined }) {
  return (
    <div className="w-56 space-y-1.5 rounded-lg border border-zinc-300 bg-white/85 p-3 shadow-sm backdrop-blur dark:border-zinc-800 dark:bg-zinc-950/85 dark:shadow-none">
      <Row label="IMEI" value={latest?.flight_id ?? "—"} />
      <Row label="MOMSN" value={latest?.momsn?.toString() ?? "—"} />
      <Row label="Transmit Time" value={latest?.transmit_time ?? "—"} />
      <Row label="Latitude" value={fmt(latest?.latitude, 4, "°")} />
      <Row label="Longitude" value={fmt(latest?.longitude, 4, "°")} />
      <Row label="CEP" value={fmt(latest?.cep, 1, " km")} />
      <Row label="Session Status" value={latest?.session_status?.toString() ?? "—"} />
    </div>
  );
}

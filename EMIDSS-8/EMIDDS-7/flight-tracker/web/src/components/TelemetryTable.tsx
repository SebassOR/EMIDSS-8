import type { TelemetryRecord } from "../types";

function fmt(value: number | undefined, digits = 2): string {
  return value === undefined || value === null ? "—" : value.toFixed(digits);
}

export function TelemetryTable({ records }: { records: TelemetryRecord[] }) {
  const rows = [...records].reverse().slice(0, 50);

  return (
    <div className="overflow-hidden rounded-lg border border-zinc-300 dark:border-zinc-800">
      <div className="max-h-72 overflow-y-auto">
        <table className="w-full text-left text-xs">
          <thead className="sticky top-0 bg-zinc-100 text-zinc-500 dark:bg-zinc-900">
            <tr>
              <th className="px-3 py-2 font-medium">MOMSN</th>
              <th className="px-3 py-2 font-medium">Transmit Time</th>
              <th className="px-3 py-2 font-medium">Lat</th>
              <th className="px-3 py-2 font-medium">Lon</th>
              <th className="px-3 py-2 font-medium">CEP</th>
              <th className="px-3 py-2 font-medium">Decoded Data</th>
            </tr>
          </thead>
          <tbody className="divide-y divide-zinc-200 tabular-nums dark:divide-zinc-800/70">
            {rows.length === 0 ? (
              <tr>
                <td colSpan={6} className="px-3 py-6 text-center text-zinc-500">
                  No telemetry received yet.
                </td>
              </tr>
            ) : (
              rows.map((r) => (
                <tr key={`${r.flight_id}-${r.momsn}-${r.timestamp}`} className="text-zinc-700 dark:text-zinc-300">
                  <td className="px-3 py-1.5">{r.momsn ?? "—"}</td>
                  <td className="px-3 py-1.5 whitespace-nowrap">{r.transmit_time ?? "—"}</td>
                  <td className="px-3 py-1.5">{fmt(r.latitude, 4)}</td>
                  <td className="px-3 py-1.5">{fmt(r.longitude, 4)}</td>
                  <td className="px-3 py-1.5">{fmt(r.cep, 1)}</td>
                  <td className="px-3 py-1.5 font-mono text-zinc-500 dark:text-zinc-400">{r.decoded_text ?? "—"}</td>
                </tr>
              ))
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
}

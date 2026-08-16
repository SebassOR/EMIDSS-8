import { useCallback, useState } from "react";
import { Header } from "./components/Header";
import { MapView } from "./components/MapView";
import { StatCards } from "./components/StatCards";
import { TelemetryDrawer } from "./components/TelemetryDrawer";
import { useLiveTelemetry } from "./hooks/useLiveTelemetry";
import { useTelemetryHistory } from "./hooks/useTelemetryHistory";
import { useTheme } from "./hooks/useTheme";
import type { TelemetryRecord } from "./types";

function mergeRecord(records: TelemetryRecord[], record: TelemetryRecord): TelemetryRecord[] {
  const withoutDuplicate = records.filter(
    (r) => !(r.flight_id === record.flight_id && r.momsn === record.momsn),
  );
  return [...withoutDuplicate, record].sort((a, b) => a.timestamp - b.timestamp);
}

export default function App() {
  const history = useTelemetryHistory();
  const [liveRecords, setLiveRecords] = useState<TelemetryRecord[]>([]);
  const [theme, toggleTheme] = useTheme();

  const onRecord = useCallback((record: TelemetryRecord) => {
    setLiveRecords((prev) => mergeRecord(prev, record));
  }, []);
  const status = useLiveTelemetry(onRecord);

  const records = liveRecords.reduce(mergeRecord, history.records);
  const latest = records[records.length - 1];

  return (
    <div className="flex h-screen flex-col bg-white dark:bg-zinc-950">
      <Header status={status} theme={theme} onToggleTheme={toggleTheme} />

      <main className="relative flex-1">
        <MapView records={records} theme={theme} />

        <div className="pointer-events-none absolute top-4 left-4 z-10">
          <div className="pointer-events-auto">
            <StatCards latest={latest} />
          </div>
        </div>

        {history.error && (
          <div className="absolute top-4 right-4 left-4 z-10 rounded-lg border border-red-300 bg-red-50/90 px-3 py-2 text-sm text-red-700 backdrop-blur sm:left-auto sm:w-80 dark:border-red-900/50 dark:bg-red-950/80 dark:text-red-300">
            {history.error}
          </div>
        )}

        <TelemetryDrawer records={records} />
      </main>
    </div>
  );
}

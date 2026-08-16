import { ChevronUp, ChevronDown } from "lucide-react";
import { useState } from "react";
import { TelemetryTable } from "./TelemetryTable";
import type { TelemetryRecord } from "../types";

export function TelemetryDrawer({ records }: { records: TelemetryRecord[] }) {
  const [open, setOpen] = useState(false);

  return (
    <div className="absolute right-0 bottom-0 left-0 z-10">
      <button
        onClick={() => setOpen((v) => !v)}
        className="flex w-full items-center justify-center gap-1.5 border-t border-zinc-300 bg-white/90 py-1.5 text-xs font-medium text-zinc-600 backdrop-blur hover:text-zinc-900 dark:border-zinc-800 dark:bg-zinc-950/90 dark:text-zinc-400 dark:hover:text-zinc-200"
      >
        {open ? <ChevronDown size={14} /> : <ChevronUp size={14} />}
        Telemetry feed ({records.length})
      </button>
      {open && (
        <div className="max-h-64 overflow-hidden border-t border-zinc-300 bg-white/95 p-2 backdrop-blur dark:border-zinc-800 dark:bg-zinc-950/95">
          <TelemetryTable records={records} />
        </div>
      )}
    </div>
  );
}

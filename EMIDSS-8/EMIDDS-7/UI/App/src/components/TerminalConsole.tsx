import { useEffect, useRef, useState } from "react";
import { listen } from "@tauri-apps/api/event";

export function TerminalConsole() {
  const [lines, setLines] = useState<string[]>([]);
  const terminalRef = useRef<HTMLDivElement | null>(null);

  useEffect(() => {
    let unlisten: (() => void) | undefined;

    async function setupListener() {
      // Listen for UART data from Rust
      unlisten = await listen<string>("uart-rx", (event) => {
        const payload = event.payload;

        setLines((currentLines) => [...currentLines, payload]);
      });
    }

    setupListener();

    return () => {
      // Remove listener when the component closes
      if (unlisten) {
        unlisten();
      }
    };
  }, []);

  useEffect(() => {
    // Keep the latest log visible
    if (terminalRef.current) {
      terminalRef.current.scrollTop = terminalRef.current.scrollHeight;
    }
  }, [lines]);

  return (
    <div className="flex flex-col h-full">
      <div className="mb-4 flex items-center justify-between border-b border-[#262626] pb-3">
        <span className="text-xs font-semibold uppercase tracking-widest text-slate-400">
          RAW TELEMETRY
        </span>
        <button
          onClick={() => setLines([])}
          className="text-[10px] font-medium uppercase tracking-wider text-slate-500 hover:text-slate-300 transition"
        >
          CLEAR
        </button>
      </div>

      <div
        ref={terminalRef}
        className="flex-1 min-h-[480px] max-h-[620px] overflow-y-auto font-mono text-xs leading-relaxed text-slate-300 space-y-1.5 pr-2"
      >
        {lines.length === 0 ? (
          <div className="text-slate-600 font-mono text-xs py-2">
            Waiting for incoming serial telemetry stream...
          </div>
        ) : (
          lines.map((line, index) => (
            <div key={`${index}-${line}`} className="break-all whitespace-pre-wrap">
              {line}
            </div>
          ))
        )}
      </div>
    </div>
  );
}

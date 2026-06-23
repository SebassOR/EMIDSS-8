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
    <section className="rounded-2xl border border-slate-700 bg-slate-900 p-5 shadow-lg">
      <div className="mb-4 flex items-center justify-between gap-4">
        <div>
          <h2 className="text-lg font-semibold text-white">Live Terminal</h2>
          <p className="text-sm text-slate-400">
            Incoming Rust UART logs are displayed in real time.
          </p>
        </div>

        <button
          onClick={() => setLines([])}
          className="rounded-lg border border-slate-700 px-4 py-2 text-sm font-medium text-slate-200 transition hover:bg-slate-800"
        >
          Clear
        </button>
      </div>

      <div
        ref={terminalRef}
        className="h-[420px] overflow-y-auto rounded-xl border border-slate-800 bg-black p-4 font-mono text-sm text-green-400"
      >
        {lines.length === 0 ? (
          <p className="text-slate-500">Waiting for incoming uart-rx data...</p>
        ) : (
          lines.map((line, index) => (
            <pre
              key={`${index}-${line}`}
              className="whitespace-pre-wrap break-words"
            >
              {line}
            </pre>
          ))
        )}
      </div>
    </section>
  );
}

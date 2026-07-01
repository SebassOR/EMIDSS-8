import { useState } from "react";
import "./App.css";
import { ConnectionManager } from "./components/ConnectionManager";
import { MissionControlPanel } from "./components/MissionControlPanel";
import { TelemetryDashboard } from "./components/TelemetryDashboard";
import { TerminalConsole } from "./components/TerminalConsole";

type Section = "connection" | "operations" | "telemetry";

function App() {
  const [activeSection, setActiveSection] = useState<Section>("connection");

  return (
    <main className="min-h-screen bg-slate-950 px-6 py-6 text-slate-100">
      <div className="mx-auto flex max-w-7xl flex-col gap-6">
        <header className="flex flex-col justify-between gap-4 rounded-2xl border border-slate-800 bg-slate-900 p-6 shadow-lg md:flex-row md:items-center">
          <div>
            <p className="text-xs font-semibold uppercase tracking-[0.3em] text-blue-400">
              EMIDSS-8 Module
            </p>
            <h1 className="mt-1 text-2xl font-bold text-white md:text-3xl">
              Flight Payload Telemetry System
            </h1>
          </div>

          <nav className="flex flex-wrap gap-2">
            <button
              onClick={() => setActiveSection("connection")}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition ${
                activeSection === "connection"
                  ? "bg-blue-600 text-white shadow-md shadow-blue-600/30"
                  : "border border-slate-700 bg-slate-950 text-slate-300 hover:bg-slate-800 hover:text-white"
              }`}
            >
              1. Connection Setup
            </button>
            <button
              onClick={() => setActiveSection("operations")}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition ${
                activeSection === "operations"
                  ? "bg-blue-600 text-white shadow-md shadow-blue-600/30"
                  : "border border-slate-700 bg-slate-950 text-slate-300 hover:bg-slate-800 hover:text-white"
              }`}
            >
              2. Operations Console
            </button>
            <button
              onClick={() => setActiveSection("telemetry")}
              className={`rounded-lg px-4 py-2 text-sm font-medium transition ${
                activeSection === "telemetry"
                  ? "bg-blue-600 text-white shadow-md shadow-blue-600/30"
                  : "border border-slate-700 bg-slate-950 text-slate-300 hover:bg-slate-800 hover:text-white"
              }`}
            >
              3. Telemetry & Graphs
            </button>
          </nav>
        </header>

        {activeSection === "connection" && (
          <div className="flex flex-col gap-6">
            <ConnectionManager onConnect={() => setActiveSection("operations")} />
          </div>
        )}

        {activeSection === "operations" && (
          <div className="flex flex-col gap-6">
            <MissionControlPanel />
            <TerminalConsole />
          </div>
        )}

        {activeSection === "telemetry" && (
          <div className="flex flex-col gap-6">
            <TelemetryDashboard />
          </div>
        )}
      </div>
    </main>
  );
}

export default App;
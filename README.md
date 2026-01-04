# DCPF
DCP File manager (Data Collector Presentor File)


# DCPF File Format (v1)

DCPF is an append-only binary log format for recording numeric samples from multiple channels with minimal write overhead.

- **Byte order:** Little-endian for all multi-byte integers
- **Write pattern:** Append-only (stream-friendly)
- **Goal:** Extremely fast writes on embedded devices (e.g., ESP32)

---

## File layout

```mermaid
flowchart LR
  H["Header - 16 bytes"] --> R0["Record 0"]
  R0 --> R1["Record 1"]
  R1 --> R2["Record 2"]
  R2 --> RN["..."]


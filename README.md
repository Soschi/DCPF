# DCPF
**DCP File Manager**  
(Data Collector Presenter File)

---

# DCPF File Format (v1)

DCPF is an **append-only binary log format** for recording numeric samples from multiple channels with **minimal write overhead**.

It is designed primarily for **high-speed embedded logging** (e.g. ESP32), while remaining simple to parse on desktop systems.

- **Byte order:** Little-endian for all multi-byte fields  
- **Write pattern:** Append-only, sequential  
- **Fixed record size:** 12 bytes  
- **Goal:** Extremely fast writes with deterministic layout  

---

## File Layout

A DCPF file consists of:
1. A fixed-size **file header**
2. Zero or more **data records**, written sequentially

---

## Header Layout (16 bytes / 128 bits)

<table>
  <tr>
    <th colspan="32" align="left">DCPF Header (16 bytes / 128 bits)</th>
  </tr>
  <tr>
    <th colspan="32" align="left">Bit offsets (within each 32-bit row)</th>
  </tr>
  <tr>
    <th>0</th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th><th>7</th>
    <th>8</th><th>9</th><th>10</th><th>11</th><th>12</th><th>13</th><th>14</th><th>15</th>
    <th>16</th><th>17</th><th>18</th><th>19</th><th>20</th><th>21</th><th>22</th><th>23</th>
    <th>24</th><th>25</th><th>26</th><th>27</th><th>28</th><th>29</th><th>30</th><th>31</th>
  </tr>

  <!-- Row 0 -->
  <tr>
    <td colspan="32" align="center"><b>magic</b> ("DCPF") (4 bytes)</td>
  </tr>

  <!-- Row 1 -->
  <tr>
    <td colspan="16" align="center"><b>version</b> (uint16)</td>
    <td colspan="16" align="center"><b>record_size</b> (uint16)</td>
  </tr>

  <!-- Row 2 -->
  <tr>
    <td colspan="32" align="center"><b>file_flags</b> (uint32)</td>
  </tr>

  <!-- Row 3 -->
  <tr>
    <td colspan="16" align="center"><b>reserved</b> (uint16)</td>
    <td colspan="16" align="center"><b>reserved</b> (uint16)</td>
  </tr>
</table>

<p><i>All multi-byte fields are little-endian.</i></p>

## Header Flags Bit Layout

<table>
  <tr>
    <th colspan="32" align="left">file_flags (uint32)</th>
  </tr>
  <tr>
    <th colspan="32" align="left">Bit offsets</th>
  </tr>
  <tr>
    <th>0</th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th><th>7</th>
    <th>8</th><th>9</th><th>10</th><th>11</th><th>12</th><th>13</th><th>14</th><th>15</th>
    <th>16</th><th>17</th><th>18</th><th>19</th><th>20</th><th>21</th><th>22</th><th>23</th>
    <th>24</th><th>25</th><th>26</th><th>27</th><th>28</th><th>29</th><th>30</th><th>31</th>
  </tr>

  <tr>
    <td colspan="32" align="center">Reserved (must be 0 in v1)</td>
  </tr>
</table>

<p><i>All bits are reserved for future use. Writers must write 0. Readers must ignore unknown bits.</i></p>

## Data Record Layout (12 bytes / 96 bits)

Each record represents **one datapoint**, or part of a datapoint in the case of 64-bit values.

<table>
  <tr>
    <th colspan="32" align="left">DCPF Record (12 bytes / 96 bits)</th>
  </tr>
  <tr>
    <th colspan="32" align="left">Bit offsets (within each 32-bit row)</th>
  </tr>
  <tr>
    <th>0</th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th><th>7</th>
    <th>8</th><th>9</th><th>10</th><th>11</th><th>12</th><th>13</th><th>14</th><th>15</th>
    <th>16</th><th>17</th><th>18</th><th>19</th><th>20</th><th>21</th><th>22</th><th>23</th>
    <th>24</th><th>25</th><th>26</th><th>27</th><th>28</th><th>29</th><th>30</th><th>31</th>
  </tr>

  <tr>
    <td colspan="16" align="center"><b>channel_id</b> (uint16)</td>
    <td colspan="16" align="center"><b>record_flags</b> (uint16)</td>
  </tr>
  <tr>
    <td colspan="32" align="center"><b>data</b> (uint32)</td>
  </tr>
  <tr>
    <td colspan="32" align="center"><b>timestamp</b> (uint32)</td>
  </tr>
</table>

_All multi-byte fields are little-endian._

---

## Field Definitions

| Field | Size | Description |
|------|-----:|-------------|
| `channel_id` | 16 bit | Channel identifier (`0–65535`) |
| `record_flags` | 16 bit | Type and control flags |
| `data` | 32 bit | Data payload (raw bits) |
| `timestamp` | 32 bit | Timestamp (e.g. milliseconds since start) |

---

## `record_flags` Bit Layout (uint16)

<table>
  <tr>
    <th colspan="16" align="left">record_flags bit layout</th>
  </tr>
  <tr>
    <th>15</th><th>14</th><th>13</th><th>12</th><th>11</th><th>10</th><th>9</th><th>8</th>
    <th>7</th><th>6</th><th>5</th><th>4</th><th>3</th><th>2</th><th>1</th><th>0</th>
  </tr>
  <tr>
    <td colspan="11" align="center">Reserved (must be 0)</td>
    <td align="center"><b>CONT</b></td>
    <td colspan="4" align="center"><b>TYPE</b></td>
  </tr>
</table>

- **TYPE** (bits 0–3): base data type  
- **CONT** (bit 5): record continues in the next record (used for 64-bit values)  
- Reserved bits must be `0` in v1

---

## TYPE Field (bits 0–3)

Defines how the `data` field must be interpreted.

| Bits | Value | Meaning |
|-----:|-----:|--------|
| `0000` | `0x0` | Invalid (must not be used) |
| `0001` | `0x1` | UINT32 |
| `0010` | `0x2` | INT32 |
| `0011` | `0x3` | FLOAT32 (IEEE-754) |
| `0100` | `0x4` | UINT64 (uses continuation) |
| `0101` | `0x5` | INT64 (uses continuation) |
| `0110` | `0x6` | FLOAT64 (uses continuation) |
| `0111–1111` | — | Reserved |

---

## CONT Flag (bit 5)

Indicates that the current record’s payload **continues in the next record**.

### Continuation Rules
- If `CONT = 1`:
  - The next record **must immediately follow**
  - The next record must use `channel_id = 0xFFFF`
  - The continuation record completes the payload
- Continuation records must **not** appear as the first record

---

## 64-bit Values (UINT64 / INT64 / FLOAT64)

64-bit values are stored using **two consecutive 12-byte records**.

### Base Record
- `channel_id` = real channel
- `TYPE` = UINT64 / INT64 / FLOAT64
- `CONT = 1`
- `data` = low 32 bits

### Continuation Record
- `channel_id = 0xFFFF`
- `TYPE` = same as base
- `CONT = 0`
- `data` = high 32 bits

The timestamp **must be identical** in both records.

---

## Reserved Values

- `channel_id = 0xFFFF` is reserved for continuation records
- Reserved flag bits must be written as zero
- Files with unknown flag bits set may be rejected

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
```

<!-- DCPF Record (12 bytes / 96 bits) -->
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
    <td colspan="16" align="center"><b>channel_id</b> (uint16 LE)</td>
    <td colspan="16" align="center"><b>record_flags</b> (uint16 LE)</td>
  </tr>
  <tr>
    <td colspan="32" align="center"><b>data</b> (uint32 LE)</td>
  </tr>
  <tr>
    <td colspan="32" align="center"><b>timestamp</b> (uint32 LE)</td>
  </tr>
</table>

<p><i>All multi-byte fields are little-endian.</i></p>


<!-- record_flags (16-bit) bit layout -->
<table>
  <tr>
    <th colspan="16" align="left">record_flags (uint16) bit layout</th>
  </tr>
  <tr>
    <th>15</th><th>14</th><th>13</th><th>12</th><th>11</th><th>10</th><th>9</th><th>8</th>
    <th>7</th><th>6</th><th>5</th><th>4</th><th>3</th><th>2</th><th>1</th><th>0</th>
  </tr>
  <tr>
    <td colspan="10" align="center">Reserved (must be 0)</td>
    <td align="center"><b>CONT</b></td>
    <td align="center"><b>SIGNED</b></td>
    <td colspan="4" align="center"><b>TYPE</b> (0..15)</td>
  </tr>
</table>

<ul>
  <li><b>TYPE</b> (bits 0–3): base data type</li>
  <li><b>SIGNED</b> (bit 4): integer signedness (ignored for floats)</li>
  <li><b>CONT</b> (bit 5): record continues in the next record (used for 64-bit values)</li>
  <li>Reserved bits (6–15) must be 0 in v1</li>
</ul>

# UCL NRV2d decompression for Intel 8080
NRV2d decompression algorithm implementation for Intel 8080 microprocessor. It is part of UCL library.<br><br>
**IMPORTANT**. This code does not accept original packed byte sequences's final marker.
The compression subroutine `ucl_nrv_99_compress` must be modificated to emit short final code prefix:<br>
UCL version: 1.03, file `src\n2_99.ch`, line #605, original:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x1000000));`<br>
Should be:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x100));`

Since original UCL's utility `uclpack` doesn't work properly for this decompressor, you can use `n2dpack` in `bin` folder to get correct packed stream.

#### Versions:
| Source | Size  | Speed | Description |
| :---   | :---: | :---: | :--- |
| `n2d-i80.asm` | 191 B | 100% | fastest version |
| `n2d-i80-s.asm` | 188 B | 3% slower | shorter |
| `n2d-i80-s2.asm` | 182 B | 18% slower | shorter and slow |
| `n2d-i80-s3.asm` | 169 B | 12% slower | shortest, but not always possible (uses `rst` instruction) |

### See also

* [NRV2d-pdp](https://github.com/usr38259/nrv2d-pdp) - NRV2d decompression for PDP-11
* [n2dpack](https://github.com/usr38259/nrv2d-i80/tree/main/src) - n2dpack utility

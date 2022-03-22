# UCL NRV2d decompression for Intel 8080
NRV2d decompression algorithm implementation for Intel 8080 microprocessor. It is part of UCL library.<br><br>
**IMPORTANT**. This code does not accept original packed byte sequences's final marker.
The compression subroutine `ucl_nrv_99_compress` must be modificated to emit short final code prefix:<br>
UCL version: 1.03, file `src\n2_99.ch`, line #605, original:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x1000000));`<br>
Should be:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x100));`<br>
<br>
Since original UCL's utility `uclpack` doesn't work properly for this decompressor, you can use `n2dpack` in `bin` folder to get correct packed stream.

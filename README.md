# UCL NRV2d decompression for Intel 8080
NRV2d decompression algorithm implementation for Intel 8080 microprocessor. It is part of UCL library.<br>
IMPORTANT. This code does not accept original packed byte sequences's final marker.
The compression subroutine `ucl_nrv_99_compress` must be modificated to store short final code prefix
and work properly:<br>
UCL version: 1.03, file `src\n2_99.ch`, line #605, original:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x1000000));`<br>
Should be:<br>
`code_prefix_ss12(c, UCL_UINT32_C(0x100));`<br>

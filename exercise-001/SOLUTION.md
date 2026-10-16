# Lösung

## Memorymap

Eine möglich Memory Map, da Linux standardmäßig eine address randomization verwendet.

Adresse                        Offset    Segment   Variable                        Größe   Wert
──────────────────────────────────────────────────────────────────────────────────────────────────
0x5fd04f7e5278                 +0x0278   .rodata   rodata                          4 B     42
0x5fd04f7e5280                 +0x0280   .rodata   rodata2                         8 B     42.3
                                         ···  (Lücke: .eh_frame, GOT etc.)  ···
0x5fd04f7e7028                 +0x2028   .data     data                            4 B     4711
0x5fd04f7e7030                 +0x2030   .data     data2                           8 B     0.815
0x5fd04f7e7038                 +0x2038   .data     main()::whoami (static)         4 B     32
0x5fd04f7e703c                 +0x203c   .data     returnAStaticValue()::whoami    4 B     78 → 101 → 123
0x5fd04f7e7044                 +0x2044   .bss      bss                             4 B     0
0x5fd04f7e7048                 +0x2048   .bss      bss2                            4 B     0
                                         ···  (riesige Lücke, ca. 0x2d4…)  ···
0x5fd07d22a6c0                 (Heap)    Heap      *heap (new double)              8 B     47.11
                                         ···  (riesige Lücke)  ···
0x7ffca31022e8                 (Stack)   Stack     returnAPointer()::pointerMe     4 B     45 (danach ungültig!)

 hohe Adressen   ┌──────────────────────────┐
                 │ Stack  (0x7ffc…)         │  pointerMe (lokal, automatisch)
                 │   ↓ wächst nach unten    │  (auch ptr, heap als Zeiger selbst)
                 │                          │
                 │        ···               │
                 │                          │
                 │   ↑ wächst nach oben     │
                 │ Heap   (0x5fd07d22a6c0)  │  *heap = 47.11 (new double)
                 ├──────────────────────────┤
                 │        ···               │
                 ├──────────────────────────┤
                 │ .bss   (0x…7044)         │  bss, bss2 (alles 0)
                 ├──────────────────────────┤
                 │ .data  (0x…7028)         │  data, data2, beide static whoami
                 ├──────────────────────────┤
                 │ .rodata(0x…5278)         │  rodata, rodata2 (const)
                 ├──────────────────────────┤
                 │ .text  (Code)            │
 niedrige Adr.   └──────────────────────────┘

 ## Analyse der Binärdatei mit readelf

```sh
$ readelf -SW ./build/bin/exercise-001

There are 40 section headers, starting at offset 0x12218:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .note.gnu.property NOTE            0000000000000350 000350 000020 00   A  0   0  8
  [ 2] .note.gnu.build-id NOTE            0000000000000370 000370 000024 00   A  0   0  4
  [ 3] .interp           PROGBITS        0000000000000394 000394 00001c 00   A  0   0  1
  [ 4] .gnu.hash         GNU_HASH        00000000000003b0 0003b0 000024 00   A  5   0  8
  [ 5] .dynsym           DYNSYM          00000000000003d8 0003d8 0000f0 18   A  6   1  8
  [ 6] .dynstr           STRTAB          00000000000004c8 0004c8 00018d 00   A  0   0  1
  [ 7] .gnu.version      VERSYM          0000000000000656 000656 000014 02   A  5   0  2
  [ 8] .gnu.version_r    VERNEED         0000000000000670 000670 000050 00   A  6   2  8
  [ 9] .rela.dyn         RELA            00000000000006c0 0006c0 0000f0 18   A  5   0  8
  [10] .rela.plt         RELA            00000000000007b0 0007b0 000048 18  AI  5  25  8
  [11] .init             PROGBITS        0000000000001000 001000 000017 00  AX  0   0  4
  [12] .plt              PROGBITS        0000000000001020 001020 000040 10  AX  0   0 16
  [13] .plt.got          PROGBITS        0000000000001060 001060 000008 08  AX  0   0  8
  [14] .text             PROGBITS        0000000000001070 001070 001222 00  AX  0   0 16
  [15] .fini             PROGBITS        0000000000002294 002294 000009 00  AX  0   0  4
  [16] .rodata           PROGBITS        0000000000003000 003000 000288 00   A  0   0  8
  [17] .eh_frame_hdr     PROGBITS        0000000000003288 003288 00013c 00   A  0   0  4
  [18] .eh_frame         PROGBITS        00000000000033c8 0033c8 0004ec 00   A  0   0  8
  [19] .note.ABI-tag     NOTE            00000000000038b4 0038b4 000020 00   A  0   0  4
  [20] .init_array       INIT_ARRAY      0000000000004d80 003d80 000008 08  WA  0   0  8
  [21] .fini_array       FINI_ARRAY      0000000000004d88 003d88 000008 08  WA  0   0  8
  [22] .data.rel.ro      PROGBITS        0000000000004d90 003d90 000008 00  WA  0   0  8
  [23] .dynamic          DYNAMIC         0000000000004d98 003d98 000220 10  WA  6   0  8
  [24] .got              PROGBITS        0000000000004fb8 003fb8 000030 08  WA  0   0  8
  [25] .got.plt          PROGBITS        0000000000004fe8 003fe8 000030 08  WA  0   0  8
  [26] .data             PROGBITS        0000000000005018 004018 000028 00  WA  0   0  8
  [27] .bss              NOBITS          0000000000005040 004040 000010 00  WA  0   0  4
  [28] .comment          PROGBITS        0000000000000000 004040 000047 01  MS  0   0  1
  [29] .debug_info       PROGBITS        0000000000000000 004087 005912 00      0   0  1
  [30] .debug_abbrev     PROGBITS        0000000000000000 009999 0005fc 00      0   0  1
  [31] .debug_line       PROGBITS        0000000000000000 009f95 00098d 00      0   0  1
  [32] .debug_str        PROGBITS        0000000000000000 00a922 004e8a 01  MS  0   0  1
  [33] .debug_addr       PROGBITS        0000000000000000 00f7ac 000320 00      0   0  1
  [34] .debug_line_str   PROGBITS        0000000000000000 00facc 00036e 01  MS  0   0  1
  [35] .debug_rnglists   PROGBITS        0000000000000000 00fe3a 000088 00      0   0  1
  [36] .debug_str_offsets PROGBITS        0000000000000000 00fec2 000f90 00      0   0  1
  [37] .symtab           SYMTAB          0000000000000000 010e58 0007b0 18     38  23  8
  [38] .strtab           STRTAB          0000000000000000 011608 000a77 00      0   0  1
  [39] .shstrtab         STRTAB          0000000000000000 01207f 000197 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), l (large), p (processor specific)
```
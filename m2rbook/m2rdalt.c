
PanelT DesPanels[DLG_DES_COUNT + 1] = {
 { NULL,      NULL,              0, 0, 0 }, // desNum 0 is reserved

	// Conversion Designer
 { "m2rdshb", "Basic Style",     1, 0, 0 },
 { "m2rdtxb", "Basic Text",      0, 0, 0 },
 { "m2rdtxa", "Advanced Text",   0, 0, 0 },
 { "m2rdfnt", "Fonts",           6, 0, 0 },
 { "m2rdtl",  "Text Layout",     2, 0, 0 },
 { "m2rdtbl", "Tables",         11, 0, 0 },
 { "m2rdtpl", "Template",        0, 0, 0 },
 { "m2rdver", "Version",         0, 0, 0 },
 { "m2rdmf",  "Multifile",       7, 0, 0 },

	// Help Designer: add Help Style panel first
 { "m2rdshb", "Help Style",      1, 0, 0 }, // 10
 { "m2rdshs", "Help Topic",      1, 0, 0 },
 { "m2rdshh", "Help Hotspot",    1, 0, 0 },
 { "m2rdsht", "Help Target",     1, 0, 0 },
 { "m2rdshm", "Help Macro",      1, 0, 0 },
 { "m2rdtbh", "Help Tables",    11, 0, 0 },  // set 11 is table styles
 { "m2rdhmk", "Help Marker",     0, 0, 0 },
 { "m2rdtxh", "Help Text",       0, 0, 0 },
 { "m2rdcnt", "Help Contents",   0, 0, 0 },
 { "m2rdhpj", "Help Compiler",   0, 0, 0 },

	// Graphic Designer: add Graphic Style panel to name styles
	//  also Properties and Export panels for size, position, DPI
	//  Creation panel to specify new para, table, or table row 
 { "m2rdgs",  "Graphic Style",  10, 0, 0 }, // 20
 { "m2rdgr",  "Basic Graphics",  3, 0, 0 },
 { "m2rdgi",  "Import Graphics", 4, 0, 0 },
#ifdef BMC_USED
 { "m2rdgrb", "Bitmaps",         9, 0, 0 },  // set 9 is bitmap chars
#else
 { "m2rdgrb", "Bitmaps",         3, 0, 0 },
#endif
#ifdef REFF_USED
 { "m2rdgra", "Graphic Frames",  8, 0, 0 },  // set 8 is reference frames
#else
 { "m2rdgra", "Graphic Frames",  3, 0, 0 },
#endif
 { "m2rdgt",  "Graphic Text",    5, 0, 0 },
 { "m2rdgsh", "Help Graphics",  10, 0, 0 },  // set 10 is graphic styles
 { "m2rdgsw", "Word Graphics",  10, 0, 0 },
 { "m2rdgsn", "HTML Graphics",  10, 0, 0 },

	// HTML Designer
 { "m2rdsnb", "HTML Style",      1, 0, 0 }, // 29
 { "m2rdsnl", "HTML List",       1, 0, 0 },
 { "m2rdsnc", "HTML Char",       1, 0, 0 },
 { "m2rdsnp", "HTML Para",       1, 0, 0 },
 { "m2rdsnm", "HTML Macro",      1, 0, 0 },
 { "m2rdtbn", "HTML Tables",    11, 0, 0 },

};

PropSetT DesPropSets[] = {
	{  9, { 1, 2, 3, 4, 5, 6, 7, 8, 9 },  NULL },         // Conversion
	{ 10, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 }, NULL }, // Help
	{  9, { 20, 21, 22, 23, 24, 25, 26, 27  28 }, NULL }, // Graphics
	{  6, { 29, 30, 31, 32, 33, 34 }, NULL }  // HTML
};

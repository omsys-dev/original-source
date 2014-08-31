
// OmniHelp functions for ix construction, OHver 0.1


function useIx() {
	currNavCtrl = 1
	writeIxCtrlFile()
	writeIxDocFile()
	setCookies()
}

function setIxId(str) {
	currIxId = str
	writeIxCtrlFile()
	writeIxDocFile()
	setCookies()
}


function writeIxCtrlFile() {
	if ((currIxId == lastIxId) && (lastNavCtrl == 1)) {
		return
	}
	lastNavCtrl = 1
	lastIxId = currIxId
	var doc = parent.navctrl.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Nav Control: Index, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="ctrl"><table border="0" cellpadding="2">\n')
	writeIxCtrl(doc)
	doc.write('\n</table></body></html>')
	doc.close()
}

function writeIxCtrl(doc) {
	var str = '<tr>'
	ctrlCount = 1
	if (includeToc) {
		ctrlCount++
		str += '<th class="nav" width="50"><a href="javascript:parent.ctrl.useToc()">Contents</a></th>'
	}
	str += '<th class="navx" width="40">Index</th>'
	if (includeSearch) {
		ctrlCount++
		str += '<th class="nav" width="40"><a href="javascript:parent.ctrl.useSearch()">Search</a></th>'
	}
	if (includeRel) {
		ctrlCount++
		str += '<th class="nav" width="50"><a href="javascript:parent.ctrl.useRel()">Related</a></th>'
	}
	str += '</tr><tr><td class="navx"'
	if (ctrlCount > 1) {
		str += 'colspan="' + ctrlCount + '"'
	}
	str += '>'
	doc.writeln(str)
	writeIxList(doc)
	doc.writeln('</td></tr>')
}


function writeIxList(doc) {
	var dat, ch, curr, str
	var last = 0

	if (!ixItems) {
		doc.writeln("Unavailable")
	}
	if (currIxId == "0") {  // starting, set to first entry
		dat = ixItems[0]
		ch = dat[1].charAt(0).toUpperCase()
		if ((ch >= "0") && (ch <= "9")) {
			currIxId = "Num"
		} else if ((ch < "A") || (ch > "Z")) {
			currIxId = "Sym"
		} else {
			currIxId = ch
		}
	}
	for (var num = 0 ; num < ixItems.length ; num++) {
		dat = ixItems[num]
		if (dat[0] != 1) {
			continue
		}
		ch = dat[1].charAt(0).toUpperCase()
		if ((ch >= "0") && (ch <= "9")) {
			curr = "Num"
		} else if ((ch < "A") || (ch > "Z")) {
			curr = "Sym"
		} else {
			curr = ch
		}
		if (curr != last) {
			if (curr == currIxId) {
				str = '<span class="currix">'
			} else {
				str = '<a href="javascript:parent.ctrl.setIxId' + "('" + curr + "')" + '">'
			}
			str += curr
			if (curr == currIxId) {
				str += '</span> '
			} else {
				str += '</a> '
			}
			doc.writeln(str)
			last = curr
		}
	}
}


function writeIxDocFile() {
	if ((currIxId == lastDocIxId) && (lastNavDoc == 1)) {
		return
	}
	lastNavDoc = 1
	lastDocIxId = currIxId
	var doc = parent.nav.document
	doc.open('text/html', "replace")
	doc.write('<html><head><title>OmniHelp Index File, OHver 0.1</title>\n')
	parent.ctrlCSS(doc)
	doc.write('</head>\n<body class="nav">\n')
	writeIxItems(doc)
	doc.write('\n</body></html>')
	doc.close()
	setTimeout("parent.nav.scrollTo(0,0)", 1000)
}

function writeIxItems(doc) {
	var dat, ch
	var writing = false

	for (var num = 0 ; num < ixItems.length ; num++) {
		dat = ixItems[num]
		if (dat[0] == 1) {
			writing = false
			ch = dat[1].charAt(0).toUpperCase()
			if ((ch >= "0") && (ch <= "9")) {
				if (currIxId == "Num") {
					writing = true
				}
			} else if ((ch < "A") || (ch > "Z")) {
				if (currIxId == "Sym") {
					writing = true
				}
			} else {
				if (currIxId == ch) {
					writing = true
				}
			}
		}
		if (writing) {
			writeOneIxItem(doc, dat)
		}
	}
}

function writeOneIxItem(doc, dat) {
	if (!checkCondIx(dat)) {
		return
	}
	var str = '<p class="toc' + dat[0] + '">'
	if (dat.length == 2) {
		if (dat[0] == 1) {
			str += '<a name="' + escape(dat[1]) + '"></a>'
		}
		str += dat[1]
	} else {
		str += '<a href="'
		if (dat[1].charAt(0) == "#") {  // for See and See Also entries
			str += '#' + escape(dat[1].substr(1)) + '">' + dat[2]
		} else {
			var num = dat[2]
			str += 'javascript:parent.ctrl.setDoc(' + num + ',\'' + dat[1] + '\')">' + tocItems[num][1]
		}
		str += '</a>'
	}
	str += '</p>'
	doc.writeln(str)
}


// end of ixcode.js


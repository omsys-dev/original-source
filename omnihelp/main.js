
// OmniHelp JS code referenced in head of all content files, OHver 0.1

if (top == self) {
	window.location.href = "index.htm"
}

if (opener) {
	opener.parent.mainCSS(document)
} else if (parent) {
  parent.mainCSS(document)
	parent.ctrl.mainChanged()
}

function sec(item, wide, high, param) {
	if (!opener) {
		parent.ctrl.secWin(item, wide, high, param)
	}
}

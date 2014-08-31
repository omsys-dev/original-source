
// OmniHelp JS code referenced in head of all content files, OHver 0.1

if (top == self) {
	var url = window.location.href
	var str = 'ohframe.htm#' + url.substr(url.lastIndexOf('/') + 1)
	window.location.replace(str)
} else if (opener) {
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

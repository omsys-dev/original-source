function syncToc(url) {
	if (parent.ctrl.usingToc == true) {
	  parent.nav.location.replace("./toc/t" + url)
	}
	parent.ctrl.currToc = url
}

function mainLoaded() {
	if (parent.ctrl.usingToc && (parent.ctrl.noTocReload == false)) {
	  parent.nav.location.replace(parent.ctrl.currToc)
	}
	parent.ctrl.noTocReload = false
}

document.write('<link rel="stylesheet" href="')
if (parent.ctrl.usingSans == false) {
	document.write('serif.css')
} else {
	document.write('main.css')
}
document.write('" type="text/css" />')

if (parent.ctrl.usingProc == true) {
	document.writeln("<style><!--")
	document.writeln('  span.proc { display: inline }')
	document.writeln("--></style>")
}


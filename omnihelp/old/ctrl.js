var tocdataName = "tocdata.htm"
var currToc = "toc.htm"
var usingToc = true
var currTocId = 0
var clickInToc = false
var noTocReload = false

var currIx = "ix.htm"
var usingIx = false

var currSearch = "search.htm"
var usingSearch = false
var lastSearch = "target"
var quickSearch = false

var currGloss = "gloss.htm"
var usingGloss = false

var usingSans = true

var usingProc = true

function setProc(tf) {
	usingProc = tf
	parent.main.location.replace(parent.main.location.href)
}

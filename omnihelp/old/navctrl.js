
function useToc() {
	parent.nav.location.replace(parent.ctrl.currToc)
	location.replace("navctrl.htm")
	parent.ctrl.usingToc = true
	parent.ctrl.usingIx = false
	parent.ctrl.usingSearch = false
	parent.ctrl.usingGloss = false
}

function useIx() {
	parent.nav.location.replace(parent.ctrl.currIx)
	location.replace("navix.htm")
	parent.ctrl.usingToc = false
	parent.ctrl.usingIx = true
	parent.ctrl.usingSearch = false
	parent.ctrl.usingGloss = false
}

function useSearch() {
	parent.nav.location.replace(parent.ctrl.currSearch)
	location.replace("navsearch.htm")
	parent.ctrl.usingToc = false
	parent.ctrl.usingIx = false
	parent.ctrl.usingSearch = true
	parent.ctrl.usingGloss = false
}

function useGloss() {
	parent.nav.location.replace(parent.ctrl.currGloss)
	location.replace("navgloss.htm")
	parent.ctrl.usingToc = false
	parent.ctrl.usingIx = false
	parent.ctrl.usingSearch = false
	parent.ctrl.usingGloss = true
}

function getPrev() {
	var sel = parent.work.tocSel
	if (sel > 0) {
		sel--
	}
	var item = parent.work.tocItems[sel]
	parent.main.location.href = item[2]
}

function getNext() {
	var sel = parent.work.tocSel
	var prev = parent.work.tocItems[sel]
	if (sel < (parent.work.tocItems.length - 1)) {
		sel++
	}
	var item = parent.work.tocItems[sel]
	while ((item[2] == prev[2]) && (sel < (parent.work.tocItems.length - 1))) {
		sel++
		item = parent.work.tocItems[sel]
	}
	parent.nav.location.replace(parent.ctrl.currToc)
	parent.ctrl.currTocId = sel
	parent.ctrl.clickInToc = true
	parent.ctrl.noTocReload = true
	parent.main.location.href = item[2]
}

function doSearch(quick) {
	parent.ctrl.lastSearch = document.navSearch.searchstring.value
	if (quick == true) {
		parent.ctrl.quickSearch = true
		parent.nav.location.replace(parent.ctrl.currSearch)
	}
}

function setSearch() {
	document.navSearch.searchstring.value = parent.ctrl.lastSearch
}




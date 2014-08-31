
if (parent.work.tocItems) {
	if (parent.work.tocItems.length == 0) {
		document.writeln('<p class="toc1">Contents empty.</p>')
	} else {
		parent.ctrl.writeTocItems(document)
	}
} else {
	document.writeln('<p class="toc1">Contents not available.</p>')
}

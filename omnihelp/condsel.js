
//Put in body of ctrl.htm:

&nbsp;Show:&nbsp;
<script language="JavaScript">
<!--
	writeCondSel(document)
// -->
</script>


// Put in main.js:

parent.ctrl.writeCondCss(document)


//Put in ctrlset.js:

// example of additional option, conditional display
var usingCond = false
var condList = [
 [1, "proc", "Procedure"],
 [0, "hideme", "Hidden" ],
 [1, "showme", "Shown" ],
 ]


//Put in ctrl.js:


// functions for condition (infotype) control

// write form elements used to select conditions
function writeCondSel(doc) {
	var str = '<select name="CondList" multiple="multiple" size="0">\n'
	for (var i = 0 ; i < condList.length ; i++) {
		str += '<option '
		if (condList[i][0] == 1) {
			str += 'selected="selected" '
		}
		str += 'value="' + condList[i][1] + '">' + condList[i][2] + '</option>\n'
	}
	str += '</select>\n<input type="button" value="Set" onClick="setCond()" />\n'
	doc.write(str)
}

// respond to Set button press
function setCond() {
	usingCond = true
	for (var i = 0 ; i < condList.length ; i++) {
		condList[i][0] = parent.topnav.document.ctrlForm.CondList.options[i].selected ? 1 : 0
	}
	parent.main.location.reload(true)
	if (currNavCtrl == 0) {
		writeTocDocFile()
	}
}

// write CSS style for body page to display conditional spans
function writeCondCss(doc) {
	if (!usingCond) {
		return
	}
	var str = '<style><!--'
	for (var i = 0 ; i < condList.length ; i++) {
		str += '  span.' + condList[i][1] + ', div.' + condList[i][1]
		str += ' { display: '
		str += condList[i][0] ? 'inline' : 'none'
		str += ' }'
	}
	str += '--></style>'
	doc.write(str)
}

function checkCondToc(dat) {
	if (!usingCond) {
		return true
	}
	if ((dat.length > 3) && condList.length) { // has conditions
		for (var i = 0; i < condList.length; i++) {
			if ((dat[3].indexOf(condList[i][1]) != -1) && (condList[i][0] == 0)) {
				return false
			}
		}
	}
	return true
}

function checkCondIx(dat) {
	return true
}


// end of cond code

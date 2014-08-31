// runcond.js provides runtime changing of conditions shown
/*
var CondAttrs = [  // set when file is written by code
	[ "audience", [ [ "novice", 0], ["expert", 0], ["admin", 1]]],
	[ "product", [ [ "m2g", 0], ["d2g", 0]]]
];
*/
var DialogDoc = null;

function showRes()
{
	var str = "";
	if (dialogWin.returnedValue) {
		str = dialogWin.returnedValue;
	}
	else {
		str = "No returned value.";
	}
	//alert(str);
	UpdateCondAttrs(str);
}

function SetCondAttrs()
{
	// when invoked from doc, put up dialog
	var aname = new Array;
	var avalue = new Array;
	var win = dialogWin.openSimDialog('', 250, 300, showRes);
	var doc = win.document;

	// set dialog to current attrs and values, checkbox for hide
	DialogDoc = doc;

	// wrire start of doc up to first table
	doc.write('<html><head><title>Conditions Dialog</title>\n');
/*
	doc.write('<script language="JavaScript">');
	doc.write('function closeme() { dialogWin.win.close(); }\n');
	doc.write('function handleOK() {\n');
	doc.write('if (opener && !opener.closed) { opener.transferData(); opener.dialogWin.returnFunc(); }\n');
	doc.write('else { alert("You have closed the main window.\n\nNo action will be taken on the choices in this dialog box."); }\n');
	doc.write('closeme(); return false; }\n');
	doc.write('function handleCancel() { closeme(); return false; }\n');
	doc.write('<\/scr');
	doc.write('ipt>\n');
*/
	doc.write('</head>\n');
	doc.write('<body><h3>Select conditions to hide:</h3><hr/><form name="conds">\n');

	for (var n = 0; n < CondAttrs.length; n++) {
		// set dialog attr name, = aname;
		aname = CondAttrs[n];
		avalue = aname[1];
		// write start of table and caption
		doc.write('<table id="');
		doc.write(aname[0]);
		doc.write('" width="150"><tr><td align="left"><input type="hidden" value="');
		doc.write(aname[0]);
		doc.write('" />For ');
		doc.write(aname[0]);
		doc.write(':</td></tr>\n');

		for (var v = 0; v < avalue.length; v++) {
			// write one table row
			doc.write('<tr><td align="left"><input type="checkbox" id="');
			// set dialog attr value, = avalue[v][0];
			doc.write(avalue[v][0]);
			doc.write('" name="');
			doc.write(avalue[v][0]);
			doc.write('" value="');
			doc.write(avalue[v][0]);
			// set dialog checkbox per hide value, = avalue[v][1];
			if (avalue[v][1]) {
				doc.write('" checked />');
			}
			else {
				doc.write('"/>');
			}
			doc.write(avalue[v][0]);
			doc.write('</td></tr>\n');
		}
		// write end of table
		doc.write('</table>\n');

	}
	// write button table and end of doc
	doc.write('<table width="220"><tr><td align="right">\n');
	doc.write('<input type="button" value="Cancel" onclick="opener.handleCancel()">\n');
	doc.write(' &nbsp;<input type="button" value=" OK " onclick="opener.handleOK()">\n');
	doc.write('</td></tr></table>\n');
	doc.write('</form></body></html>\n');
	doc.close();

	// show dialog, modally
	dialogWin.checkModal();
}

function UpdateCondAttrs(str)
{
	// update any changed Hide settings, and set document
	var hide = false;
	var ckstr = "";
	for (var n = 0; n < CondAttrs.length; n++) {
		var aname = CondAttrs[n];
		var avalue = aname[1];
		for (var v = 0; v < avalue.length; v++) {
			// set hide per dialog value
			ckstr = aname[0] + "_" + avalue[v][0];
			hide = (str.indexOf(ckstr) != -1);
			avalue[v][1] = hide ? 1 : 0;
		}
	}
	SetDocConditions();
}


function SetDocConditions()
{
	var array = document.getElementsByTagName("*");

	for (var i = 0; i < array.length; i++) {
	  var elem = array[i];
	  var eclass = elem.getAttribute("class");
	  if (CheckElemClass(elem, eclass)) { // true to hide
			HideElem(elem);
		}
	  else {
			ShowElem(elem);
		}
	}
}


function CheckElemClass(elem, eclass)
{
	if (!eclass) {
		return false;
	}

	// for each attr name in CondAttrs, see if mentioned in class
	//   if so, check each value; if all are 1, ret true
	// if all members with same attr name are hidden, hide
	// return true to hide elem, false to show
	var aname = new Array;
	var avalue = new Array;
	var aclass = new Array;

	for (var n = 0; n < CondAttrs.length; n++) {
		aname = CondAttrs[n];
		var str = "\\b" + aname[0] + "_\\w+\\b";
		var nl = aname[0].length + 1;
		var re = new RegExp(str, "g");
		var vmatch = false;
		aclass = eclass.match(re);
		if (aclass) {  // array of classes for this elem for this attr
			vmatch = false;
			for (var c = 0; c < aclass.length; c++) { // each class for this elem
				avalue = aname[1];
				var val = aclass[c].substring(nl);
				for (var v = 0; v < avalue.length; v++) {  // each value to check for
					if (val == avalue[v][0]) {  // value matches this class
						if (avalue[v][1] == 0) {  // not hide, include
							vmatch = true;
						}  // else assume hide
						break;  // either way, done checking this class
					}  // else no match, maybe include
				}
				if (vmatch) {  // an include
					break;
				}
				if (v == avalue.length) { // or no match to any value
					vmatch = true;
					break;
				}
			}
			if (!vmatch) {  // all values were excluded for this attr
				return true;
			}
		}
	}
	return false; // include
}




function ShowElem(elem)
{
	// remove display: none if present in style
	if (elem.style && elem.style.display) {
		if (elem.style.display == "none")
			elem.style.display = "";
	}
}

function HideElem(elem)
{
	// add display: none if not present in style
	if (elem.style && elem.style.display) {
		if (elem.style.display != "none")
			elem.style.display = "none";
	}
	else {
		elem.style.display = "none";
	}
}


// Close the dialog
function closeme() {
	dialogWin.win.close();
	DialogDoc = null;
}

// Handle click of OK button
function handleOK() {
    transferData();
    dialogWin.returnFunc();
    closeme();
    return false;
}

// Handle click of Cancel button
function handleCancel() {
    closeme();
    return false;
}


// Generic function converts form element name-value pairs
function getFormData(form) {
	var str = " ";
	var group = "";
	for (var i = 0; i < form.elements.length; i++) {
		if ((form.elements[i].type == "checkbox") && 
				(form.elements[i].checked == true))
		{
			str += group + "_" + form.elements[i].value + " ";
		}
		else if (form.elements[i].type == "hidden") {
			group = form.elements[i].value;
		}
  }
  return str;
}

// Send gathered data to the dialog window object's returnedValue property
function transferData() {
        //if (opener && !opener.closed) {
        //        opener.dialogWin.returnedValue = getFormData(document.conds)
        //}
        dialogWin.returnedValue = getFormData(dialogWin.win.document.conds)
}

// One object tracks the current modal dialog opened from this window.
var dialogWin = {
    // Since links in some browsers cannot be truly disabled, preserve
    // link click & mouseout event handlers while they're "disabled."
    // Restore when re-enabling the main window.
    linkClicks : null,
    // Event handler to inhibit Navigator 4 form element
    // and IE link activity when dialog window is active.
    deadend : function(evt) {
        if (this.win && !this.win.closed) {
            if (evt) {
                evt.preventDefault();
                evt.stopPropagation();
            }
            this.win.focus();
            return false;
        }
    },
    // Disable form elements and links in all frames.
    disableForms : function() {
        this.linkClicks = new Array();
        for (var i = 0; i < document.forms.length; i++) {
            for (var j = 0; j < document.forms[i].elements.length; j++) {
                document.forms[i].elements[j].disabled = true;
            }
        }
        for (i = 0; i < document.links.length; i++) {
            this.linkClicks[i] = {click:document.links[i].onclick, up:null};
            this.linkClicks[i].up = document.links[i].onmouseup;
            document.links[i].onclick = dialogWin.deadend;
            document.links[i].onmouseup = dialogWin.deadend;
            document.links[i].disabled = true;
        }
    },
    // Restore form elements and links to normal behavior.
    enableForms : function() {
        for (var i = 0; i < document.forms.length; i++) {
            for (var j = 0; j < document.forms[i].elements.length; j++) {
                document.forms[i].elements[j].disabled = false;
            }
        }
        for (i = 0; i < document.links.length; i++) {
            document.links[i].onclick = this.linkClicks[i].click;
            document.links[i].onmouseup = this.linkClicks[i].up;
            document.links[i].disabled = false;
        }
    },
    // Disable form elements.
    blockEvents : function() {
        this.disableForms();
        window.onfocus = dialogWin.checkModal;
        document.body.onclick = dialogWin.checkModal;
        addEvent(document, "click", dialogWin.checkModal, true);
        addEvent(document, "mousemove", dialogWin.checkModal, true);
    },
    // As dialog closes, restore the main window's original
    // event mechanisms.
    unblockEvents : function() {
        this.enableForms();
        window.onfocus = null;
        removeEvent(document, "click", dialogWin.checkModal, true);
        removeEvent(document, "mousemove", dialogWin.checkModal, true);
    },
    // Generate a modal dialog.
    // Parameters:
    // url -- URL of the page/frameset to be loaded into dialog
    // width -- pixel width of the dialog window
    // height -- pixel height of the dialog window
    // returnFunc -- reference to the function (on this page)
    // that is to act on the data returned from the dialog
    // args -- [optional] any data you need to pass to the dialog
    openSimDialog : function(url, width, height, returnFunc, args) {
        if (!this.win || (this.win && this.win.closed)) {
            // Initialize properties of the modal dialog object.
            this.url = url;
            this.width = width;
            this.height = height;
            this.returnFunc = returnFunc;
            this.args = args;
            this.returnedValue = "";
            // Keep name unique.
            this.name = (new Date()).getSeconds().toString();
            // Assemble window attributes and try to center the dialog.
            if (window.screenX) { // Moz, Saf, Op
                // Center on the main window.
                this.left = window.screenX +
                   ((window.outerWidth - this.width) / 2);
                this.top = window.screenY +
                   ((window.outerHeight - this.height) / 2);
                var attr = "screenX=" + this.left +
                   ",screenY=" + this.top + ",width=" +
                   this.width + ",height=" + this.height + ",scrollbars";
            } else if (window.screenLeft) { // IE 5+/Windows
                // Center (more or less) on the IE main window.
                // Start by estimating window size,
                // taking IE6+ CSS compatibility mode into account
                var CSSCompat = (document.compatMode && document.compatMode != "BackCompat");
                window.outerWidth = (CSSCompat) ? document.body.parentElement.clientWidth :
                    document.body.clientWidth;
                window.outerHeight = (CSSCompat) ? document.body.parentElement.clientHeight :
                    document.body.clientHeight;
                window.outerHeight -= 80;
                this.left = parseInt(window.screenLeft+
                   ((window.outerWidth - this.width) / 2));
                this.top = parseInt(window.screenTop +
                   ((window.outerHeight - this.height) / 2));
                var attr = "left=" + this.left +
                   ",top=" + this.top + ",width=" +
                   this.width + ",height=" + this.height + ",scrollbars";
            } else { // all the rest
                // The best we can do is center in screen.
                this.left = (screen.width - this.width) / 2;
                this.top = (screen.height - this.height) / 2;
                var attr = "left=" + this.left + ",top=" +
                   this.top + ",width=" + this.width +
                   ",height=" + this.height + ",scrollbars";
            }
            // Generate the dialog and make sure it has focus.
            this.win=window.open(this.url, this.name, attr);
            this.win.focus();
        } else {
            this.win.focus();
        }
				return this.win;
    },
    // Invoked by focus event handler of EVERY frame,
    // return focus to dialog window if it's open.
    checkModal : function() {
        setTimeout("dialogWin.finishChecking()", 50);
        return true;
    },
    finishChecking : function() {
        if (this.win && !this.win.closed) {
            this.win.focus();
        }
    }
};

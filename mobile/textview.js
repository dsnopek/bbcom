
window.onload = function () {
    var expanded = false,
        shown = false,
        bodyNode = document.getElementById('lingwo-body'),
        entryNode = document.getElementById('lingwo-entry'),
        activeWordNode = null;

    function setHeight(node, value) {
        node.style.visibility = value == 0 ? 'hidden' : 'visible';
        node.style.height = value + 'px';
    }

    function calcResize(animate) {
        var browserHeight = window.innerHeight,
            entrySize = shown ? (expanded ? browserHeight : 35) : 0,
            bodySize = browserHeight - entrySize;
        
        [entryNode, bodyNode].forEach(function (node) {
            node.style.webkitTransitionProperty = animate ? 'height, visibility' : '';
            node.style.webkitTransitionDuration = animate ? '1s, 1s' : '';
            node.style.webkitTransitionTimingFunction = animate ? 'ease-out, ease' : '';
        });

        setHeight(entryNode, entrySize);
        setHeight(bodyNode, bodySize);

        //setTimeout(function () { if (myScroll) myScroll.refresh() }, 0);
    };
    calcResize();

    // initialize the iScroll
    myScroll = new iScroll('scroller', {
        checkDOMChanges: false,
        hScrollbar: true,
        //desktopCompatibility: true
    });
    setTimeout(function () { myScroll.refresh() }, 0);

    function getText(node) {
        var s = "", child = node.firstChild;
        while (child != null) {
            if (child.nodeType == 1) {
                s += getText(child);
            }
            else if (child.nodeType == 3 || child.nodeType == 4) {
                s += child.data;
            }
            child = child.nextSibling;
        }
        return s;
    }

    function toggleEntryVisibility(show) {
        if (typeof show === 'undefined') {
            show = !shown;
        }
        shown = show;
        calcResize(true);
    }

    function toggleEntryExpanded(expand) {
        if (typeof expand === 'undefined') {
            expand = !expanded;
        }
        expanded = expand;
        calcResize(true);
    }

    function generateEntryHTML(entry, abbr) {
        if (typeof abbr === 'undefined') {
            abbr = false;
        }
        return entry.headword;
    }

    function wordClick(evt) {
        if (evt.target.tagName.toLowerCase() != 'word') {
            // clear activeWordNode
            activeWordNode.className = '';
            activeWordNode = null;

            toggleEntryVisibility(false);

            return;
        }

        var node = evt.target,
            headword = node.getAttribute('headword'),
            pos = node.getAttribute('pos'),
            sense = node.getAttribute('sense') || null,
            form = node.getAttribute('form') || null;

        if (!headword) {
            headword = getText(node);
        }

        if (activeWordNode !== null) {
            activeWordNode.className = '';
        }
        activeWordNode = node;
        activeWordNode.className = 'active';

        var entryJson = LingwoInterface.loadEntry(headword, pos, sense),
            entry = eval('('+entryJson+')');
        entryNode.innerHTML = generateEntryHTML(entry, true);
        toggleEntryVisibility(true);
    }

    // setup all events
    document.addEventListener('touchmove', function (evt) { evt.preventDefault(); });
    bodyNode.onclick = wordClick;
    entryNode.onclick = function (evt) { toggleEntryExpanded() };
    window.onresize = function (evt) { calcResize(); };
};


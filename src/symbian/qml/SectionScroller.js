var _sectionData = [];
var _sections = [];

function initSectionData(list) {
    if (!list || !list.model) return;
    _sectionData = [];
    _sections = [];
    var current = "",
        prop = list.section.property

    if (list.model.hasOwnProperty("model")) {
        for (var i = 0, count = list.model.count; i < count; i++) {
            current = list.model.model.data(i, list.model.rootIndex, prop);
            _sections.push(current);
            _sectionData.push({ index: i, header: current });
        }
    }
    else {
        for (var i = 0, count = list.model.count; i < count; i++) {
            current = list.model.data(i, prop);
            _sections.push(current);
            _sectionData.push({ index: i, header: current });
        }
    }
}

function closestSection(pos) {
    var tmp = (_sections.length) * pos;
    var val = Math.ceil(tmp) // TODO: better algorithm
    val = val < 2 ? 1 : val;
    return _sections[val-1];
}

function indexOf(sectionName) {
    var val = _sectionData[_sections.indexOf(sectionName)].index;
    return val === 0 || val > 0 ? val : -1;
}

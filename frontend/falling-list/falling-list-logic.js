.pragma library

var listData = {}
var currentPath = []

function getData() {
    return listData;
}

function getCurrentPath() {
    return currentPath;
}

function isLeaf(obj) {
    return obj && !obj.items;
}


function getCurrentLevel() {
    let currentLevel = listData;
    for (let i = 0; i < currentPath.length; i++) {
        const path = currentPath[i];

        if (currentLevel[path]?.items) {
            currentLevel = currentLevel[path].items;
        } else {
            break;
        }
    }

    return currentLevel;
}

function goDeeper(key, leafSelectedCallback, dirSelectedCallback, listModel) {
    const currentLevel = getCurrentLevel();
    const next = currentLevel[key];

    if (isLeaf(next)) {
        leafSelectedCallback?.(next, currentPath);
    } else {
        currentPath.push(key); // ✅ Мутируем вместо concat

        updateListModel(listModel);
        dirSelectedCallback?.(key);
    }
}

function goBack(listModel) {
    if (currentPath.length > 0) {
        currentPath.pop(); // ✅ Мутируем вместо slice
        updateListModel(listModel);
    }
}

function addItems(objectList, listModel) {
    if (!objectList?.length) return;

    // ✅ Мутируем существующие данные вместо копирования
    let needsUpdate = false;

    let level = listData;

    for (let i = 0; i < objectList.length; i++) {
        const item = objectList[i];

        if (item?.path?.length) {
            for (let j = 0; j < item.path.length; j++) {
                level = level[path[j]];
            }
        } else {
            level = level[item.key] || {};
        }

        if (item?.data?.items) {
            if (!level) {
                level = item.data;
            } else {
                Object.assign(level.items, item.data.items);
            }
        }

        const currentPathStr = currentPath.join('/');

        if (currentPathStr === item?.path?.join('/') || currentPathStr === item.key) {
           needsUpdate = true;
        }
    }

    if (needsUpdate && listModel) {
        updateListModel(listModel, "21"); // Обновляем если изменения в текущем уровне
    }
}

function setList(objectList) {
    listData = objectList || {};
}

function updateListModel(listModel, beb = "") {
    if (!listModel) return;

    // ✅ Кэширование - избегаем лишних обновлений
    const currentPathStr = currentPath.join('/');
    console.log("cache1: ", currentPathStr);

    const currentLevel = getCurrentLevel();

    // ✅ Быстрое заполнение
    const keys = Object.keys(currentLevel);
    const items = [];

    for (let i = 0; i < keys.length; i++) {
        const k = keys[i];

        items[i] = {
            key: k,
            displayValue: currentLevel[k]?.name || k,
        };
    }

    listModel.clear();

    listModel.append(items);

    // ✅ Сохраняем в кэш
}

function getItemData(key) {
    return getCurrentLevel()[key];
}

/**
 * preserve_api.js
 * 功能：管理 API Key 存储 (data/api.txt) 与聊天历史 (data/history/)
 * 依赖：需在支持 File System Access API 的现代浏览器中运行（Chrome/Edge 86+）
 * 目录结构：
 *   www/
 *   ├── index.html
 *   ├── css/
 *   ├── data/
 *   │   ├── api.txt
 *   │   └── history/
 *   │       ├── session_xxx.json
 *   │       └── ...
 *   └── preserve_api.js (本文件)
 */

(function(global) {
    'use strict';

    const hasBridge = typeof window.ADNative !== 'undefined' && 
                      typeof window.ADNative.callNative === 'function';

    function callNative(method, params) {
        if (!hasBridge) {
            return localStorageFallback(method, params);
        }
        return window.ADNative.callNative(method, params);
    }

    function localStorageFallback(method, params) {
        const prefix = 'deepseek_';
        const stores = {
            saveKey: () => { localStorage.setItem(prefix + 'api_key', params.key); return Promise.resolve(true); },
            getKey: () => Promise.resolve(localStorage.getItem(prefix + 'api_key')),
            clearKey: () => { localStorage.removeItem(prefix + 'api_key'); return Promise.resolve(true); },
            saveHistory: () => { localStorage.setItem(prefix + 'history_' + params.sessionId, JSON.stringify(params.data)); return Promise.resolve(true); },
            loadHistory: () => Promise.resolve(JSON.parse(localStorage.getItem(prefix + 'history_' + params.sessionId) || 'null')),
            listHistories: () => {
                let list = [];
                for (let i = 0; i < localStorage.length; i++) {
                    let key = localStorage.key(i);
                    if (key && key.startsWith(prefix + 'history_')) {
                        let id = key.replace(prefix + 'history_', '');
                        try {
                            let data = JSON.parse(localStorage.getItem(key));
                            list.push({ id: id, timestamp: data.timestamp });
                        } catch(e) {}
                    }
                }
                return Promise.resolve(list);
            },
            deleteHistory: () => { localStorage.removeItem(prefix + 'history_' + params.sessionId); return Promise.resolve(true); }
        };
        return (stores[method] || (() => Promise.reject('unknown method')))();
    }

    global.PreserveAPI = {
        init: () => Promise.resolve(),
        saveKey: (key) => callNative('saveKey', { key: key }),
        getKey: () => callNative('getKey', {}),
        clearKey: () => callNative('clearKey', {}),
        saveHistory: (id, data) => callNative('saveHistory', { sessionId: id, data: data }),
        loadHistory: (id) => callNative('loadHistory', { sessionId: id }),
        listHistories: () => callNative('listHistories', {}),
        deleteHistory: (id) => callNative('deleteHistory', { sessionId: id })
    };
})(window);
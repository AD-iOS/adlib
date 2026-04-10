```www/index.html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
    <title>DeepSeek · 智能助理</title>
    <!-- 基础样式 / 深色模式变量通过切换类实现 -->
    <link id="themeStyle" rel="stylesheet" href="css/style.css">
    <style>
        /* 全局样式 + 暗色主题变量 (style.css 仅为占位，实际变量在此定义) */
        * {
            box-sizing: border-box;
            margin: 0;
        }

        :root {
            --bg-page: #f7f9fc;
            --surface: #ffffff;
            --border-light: #e9edf2;
            --bubble-user: #0a66e9;
            --bubble-user-text: white;
            --bubble-assistant: #f0f2f5;
            --bubble-assistant-text: #1e2a3e;
            --text-secondary: #8e9aaf;
            --header-gradient-start: #0e6dfd;
            --header-gradient-end: #0a58ca;
            --input-bg: #f8fafc;
            --input-border: #e2e8f0;
            --shadow: 0 20px 35px -12px rgba(0,0,0,0.1);
            --error-bg: #ffe6e5;
            --error-text: #c62828;
        }

        body.dark {
            --bg-page: #0f1419;
            --surface: #1a232c;
            --border-light: #2a3642;
            --bubble-user: #1e6feb;
            --bubble-user-text: #ffffff;
            --bubble-assistant: #2a3743;
            --bubble-assistant-text: #e6edf3;
            --text-secondary: #9aacbc;
            --header-gradient-start: #3f8cff;
            --header-gradient-end: #1a5dc9;
            --input-bg: #1f2a34;
            --input-border: #3a4858;
            --shadow: 0 20px 35px -8px rgba(0,0,0,0.6);
            --error-bg: #4a2a2a;
            --error-text: #ffb4b4;
        }

        body {
            font-family: system-ui, -apple-system, 'Segoe UI', Roboto, 'Helvetica Neue', sans-serif;
            background: var(--bg-page);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            transition: background 0.2s;
        }

        .chat-app {
            max-width: 900px;
            width: 100%;
            background: var(--surface);
            border-radius: 28px;
            box-shadow: var(--shadow);
            overflow: hidden;
            transition: all 0.2s;
            border: 1px solid var(--border-light);
        }

        /* 头部：包含刷新/设置/新建聊天 */
        .chat-header {
            background: var(--surface);
            padding: 14px 20px;
            border-bottom: 1px solid var(--border-light);
            display: flex;
            align-items: center;
            justify-content: space-between;
            flex-wrap: wrap;
            gap: 12px;
        }

        .header-left {
            display: flex;
            align-items: center;
            gap: 12px;
        }

        .chat-header h1 {
            margin: 0;
            font-size: 1.5rem;
            font-weight: 600;
            background: linear-gradient(135deg, var(--header-gradient-start), var(--header-gradient-end));
            background-clip: text;
            -webkit-background-clip: text;
            color: transparent;
        }

        .badge {
            background: #eef2ff;
            padding: 4px 10px;
            border-radius: 40px;
            font-size: 0.75rem;
            color: #0a58ca;
            font-weight: 500;
        }
        body.dark .badge {
            background: #253341;
            color: #9dc6ff;
        }

        .header-actions {
            display: flex;
            gap: 8px;
        }

        .icon-btn {
            background: transparent;
            border: 1px solid var(--border-light);
            border-radius: 30px;
            width: 40px;
            height: 40px;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            font-size: 1.2rem;
            color: var(--bubble-assistant-text);
            transition: 0.2s;
            background: var(--input-bg);
        }
        .icon-btn:hover {
            background: var(--bubble-assistant);
            border-color: var(--header-gradient-start);
        }

        /* 设置面板 */
        .settings-panel {
            background: var(--surface);
            border-bottom: 1px solid var(--border-light);
            padding: 16px 20px;
            display: none;
            flex-direction: column;
            gap: 18px;
        }
        .settings-panel.show {
            display: flex;
        }
        .setting-row {
            display: flex;
            align-items: center;
            justify-content: space-between;
            flex-wrap: wrap;
        }
        .setting-label {
            font-weight: 500;
            color: var(--bubble-assistant-text);
        }
        select, .btn-small {
            padding: 8px 16px;
            border-radius: 40px;
            border: 1px solid var(--border-light);
            background: var(--input-bg);
            color: var(--bubble-assistant-text);
            font-size: 0.9rem;
            cursor: pointer;
        }
        .protocol-link {
            color: var(--header-gradient-start);
            text-decoration: underline;
            cursor: pointer;
            background: none;
            border: none;
            font-size: 0.95rem;
        }

        /* 消息区域 */
        .messages-area {
            height: 460px;
            overflow-y: auto;
            padding: 20px 20px 10px;
            background: var(--surface);
            display: flex;
            flex-direction: column;
            gap: 16px;
            scroll-behavior: smooth;
        }

        .message {
            display: flex;
            flex-direction: column;
            max-width: 85%;
            animation: fadeInUp 0.25s;
        }
        .user-message { align-self: flex-end; }
        .assistant-message { align-self: flex-start; }

        .bubble {
            padding: 12px 16px;
            border-radius: 24px;
            line-height: 1.45;
            font-size: 0.95rem;
            word-break: break-word;
            white-space: pre-wrap;
            box-shadow: 0 1px 3px rgba(0,0,0,0.05);
        }
        .user-message .bubble {
            background: var(--bubble-user);
            color: var(--bubble-user-text);
            border-bottom-right-radius: 6px;
        }
        .assistant-message .bubble {
            background: var(--bubble-assistant);
            color: var(--bubble-assistant-text);
            border-bottom-left-radius: 6px;
        }
        .message-time {
            font-size: 0.7rem;
            margin: 6px 12px 0;
            color: var(--text-secondary);
        }
        .user-message .message-time { text-align: right; }

        .input-area {
            padding: 16px 20px 20px;
            background: var(--surface);
            border-top: 1px solid var(--border-light);
            display: flex;
            gap: 12px;
            align-items: flex-end;
        }
        .input-wrapper {
            flex: 1;
            background: var(--input-bg);
            border-radius: 32px;
            border: 1px solid var(--input-border);
            display: flex;
            align-items: center;
            padding: 6px 16px;
        }
        .input-wrapper:focus-within {
            border-color: #0a66e9;
            box-shadow: 0 0 0 3px rgba(10,102,233,0.15);
        }
        #userInput {
            flex: 1;
            border: none;
            background: transparent;
            padding: 12px 8px;
            font-size: 0.95rem;
            outline: none;
            resize: none;
            font-family: inherit;
            color: var(--bubble-assistant-text);
        }
        #sendBtn {
            background: var(--bubble-user);
            border: none;
            color: white;
            width: 48px;
            height: 48px;
            border-radius: 48px;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            font-size: 1.5rem;
            box-shadow: 0 2px 6px rgba(0,0,0,0.1);
        }

        /* 历史侧栏 (简易下拉/面板) */
        .history-panel {
            padding: 8px 20px 16px;
            border-bottom: 1px solid var(--border-light);
            display: flex;
            gap: 12px;
            align-items: center;
            flex-wrap: wrap;
        }
        .history-select {
            flex: 1;
            min-width: 180px;
        }

        .error-bubble { background: var(--error-bg) !important; color: var(--error-text) !important; }

        @keyframes fadeInUp { from { opacity: 0; transform: translateY(12px); } to { opacity: 1; transform: translateY(0); } }
        @media (max-width: 560px) { .message { max-width: 92%; } }
        .messages-area::-webkit-scrollbar { width: 5px; }
        .messages-area::-webkit-scrollbar-thumb { background: #b9c4d4; border-radius: 10px; }
    </style>
</head>
<body>
<div class="chat-app">
    <div class="chat-header">
        <div class="header-left">
            <h1>DeepSeek</h1>
            <span class="badge" id="langBadge">中文</span>
        </div>
        <div class="header-actions">
            <button class="icon-btn" id="newChatBtn" title="新建聊天">➕</button>
            <button class="icon-btn" id="refreshBtn" title="刷新">🔄</button>
            <button class="icon-btn" id="settingsToggleBtn" title="设置">⚙️</button>
        </div>
    </div>

    <!-- 设置面板 -->
    <div class="settings-panel" id="settingsPanel">
        <div class="setting-row">
            <span class="setting-label" data-i18n="theme">🌓 深色模式</span>
            <select id="themeSelect">
                <option value="light">浅色</option>
                <option value="dark">深色</option>
            </select>
        </div>
        <div class="setting-row">
            <span class="setting-label" data-i18n="language">🌐 语言 / Language</span>
            <select id="languageSelect">
                <option value="zh">中文</option>
                <option value="en">English</option>
            </select>
        </div>
        <div class="setting-row">
            <span class="setting-label">📄 服务协议</span>
            <button class="protocol-link" id="showProtocolBtn">查看 DeepSeek 协议</button>
        </div>
        <div class="setting-row">
            <span class="setting-label">🔑 API Key 状态</span>
            <button class="btn-small" id="resetApiKeyBtn">重新配置 Key</button>
        </div>
    </div>

    <!-- 历史记录快速栏 -->
    <div class="history-panel">
        <select id="historySelect" class="history-select">
            <option value="">📂 加载历史聊天...</option>
        </select>
        <button class="icon-btn" id="saveHistoryBtn" style="width:auto; padding:0 16px;" title="保存当前对话">💾 保存</button>
    </div>

    <div class="messages-area" id="messagesArea">
        <!-- 动态内容 -->
    </div>
<!-- 
    <div class="input-area">
        <div class="input-wrapper">
            <textarea id="userInput" placeholder="输入你的问题..." rows="1"></textarea>
        </div>
        <button id="sendBtn" aria-label="发送">➤</button>
    </div>
-->
    <div class="input-area">
        <button id="uploadBtn" class="icon-btn" title="上传文件">📎</button>
        <div class="input-wrapper">
            <textarea id="userInput" placeholder="输入你的问题..." rows="1"></textarea>
        </div>
        <button id="sendBtn" aria-label="发送">➤</button>
    </div>
    <!-- 添加文件预览区域 -->
    <div id="filePreview" style="display: none; padding: 8px 20px; border-top: 1px solid var(--border-light); background: var(--surface);">
        <div style="display: flex; align-items: center; gap: 10px;">
            <span id="fileName">📄 </span>
            <span id="fileSize"></span>
            <button id="cancelFileBtn" class="icon-btn" style="width: auto; padding: 4px 12px;">取消</button>
        </div>
    </div>
</div>

<!-- 引入 preserve_api.js (管理api存储到data/api.txt, 并提供接口) -->
<script src="preserve_api.js"></script>
<script>
    (function(){
        "use strict";

        // ---------- 全局变量 ----------
        const messagesContainer = document.getElementById('messagesArea');
        const userInputEl = document.getElementById('userInput');
        const sendButton = document.getElementById('sendBtn');
        const themeSelect = document.getElementById('themeSelect');
        const languageSelect = document.getElementById('languageSelect');
        const settingsPanel = document.getElementById('settingsPanel');
        const settingsToggleBtn = document.getElementById('settingsToggleBtn');
        const refreshBtn = document.getElementById('refreshBtn');
        const newChatBtn = document.getElementById('newChatBtn');
        const historySelect = document.getElementById('historySelect');
        const saveHistoryBtn = document.getElementById('saveHistoryBtn');
        const showProtocolBtn = document.getElementById('showProtocolBtn');
        const resetApiKeyBtn = document.getElementById('resetApiKeyBtn');
        const langBadge = document.getElementById('langBadge');
        const uploadBtn = document.getElementById('uploadBtn');
        const filePreview = document.getElementById('filePreview');
        const fileNameSpan = document.getElementById('fileName');
        const fileSizeSpan = document.getElementById('fileSize');
        const cancelFileBtn = document.getElementById('cancelFileBtn');


        // 状态
        let conversationHistory = [ { role: 'system', content: '你是一个友好、专业的AI助手，名字叫 DeepSeek，用中文回复，回答简洁有帮助。' } ];
        let isWaitingReply = false;
        let currentApiKey = '';
        // 当前聊天会话ID (用于保存/加载)
        let currentSessionId = 'default_' + Date.now();
        let pendingFile = null;  // 存储待上传的文件信息

        // 多语言文本 (简易)
        const i18n = {
            zh: {
                welcome: '你好！我是 DeepSeek，有什么可以帮你的吗？',
                placeholder: '输入你的问题...',
                protocol: 'DeepSeek 服务协议：https://platform.deepseek.com/terms （示意）',
                saveOk: '聊天已保存',
                loadOk: '加载成功',
                newChat: '新聊天'
            },
            en: {
                welcome: 'Hello! I am DeepSeek, how can I help you?',
                placeholder: 'Type your message...',
                protocol: 'DeepSeek Terms of Service: https://platform.deepseek.com/terms',
                saveOk: 'Chat saved',
                loadOk: 'Loaded',
                newChat: 'New chat'
            }
        };
        let currentLang = 'zh';

        uploadBtn.addEventListener('click', () => {
            if (typeof ADNative !== 'undefined' && ADNative.callNative) {
                ADNative.callNative('chooseFile', {})
                    .then(result => {
                        if (result && result.success && result.fileInfo) {
                            pendingFile = result.fileInfo;
                            // 显示预览
                            fileNameSpan.innerHTML = `📄 ${pendingFile.name}`;
                            let sizeStr = pendingFile.size < 1024 ? `${pendingFile.size} B` 
                                : pendingFile.size < 1048576 ? `${(pendingFile.size / 1024).toFixed(1)} KB` 
                                : `${(pendingFile.size / 1048576).toFixed(1)} MB`;
                            fileSizeSpan.innerHTML = sizeStr;
                            filePreview.style.display = 'block';
                        }
                    })
                    .catch(err => console.error('选择文件失败:', err));
            } else {
                alert('当前环境不支持文件上传');
            }
        });
        // 取消文件选择
cancelFileBtn.addEventListener('click', () => {
            pendingFile = null;
            filePreview.style.display = 'none';
        });
        // ---------- 辅助函数 ----------
        function escapeHtml(str) {
            if (!str) return '';
            return str.replace(/[&<>]/g, m => ({'&':'&amp;','<':'&lt;','>':'&gt;'})[m]);
        }

        function scrollToBottom() {
            messagesContainer.scrollTop = messagesContainer.scrollHeight;
        }

        function getTimeStr() {
            return new Date().toLocaleTimeString([], { hour:'2-digit', minute:'2-digit' });
        }

        // 更新UI语言
        function applyLanguage(lang) {
            currentLang = lang;
            langBadge.textContent = lang === 'zh' ? '中文' : 'English';
            userInputEl.placeholder = i18n[lang].placeholder;
            const welcomeDiv = document.querySelector('#welcomeMsg');
            if (welcomeDiv) {
                welcomeDiv.querySelector('.bubble').textContent = i18n[lang].welcome;
            }
            // 简单刷新部分静态文本
        }

        // 主题切换
        function applyTheme(theme) {
            if (theme === 'dark') document.body.classList.add('dark');
            else document.body.classList.remove('dark');
            localStorage.setItem('deepseek_theme', theme);
        }

        // 显示欢迎消息 (清空并重置)
        function resetToWelcome() {
            messagesContainer.innerHTML = '';
            const welcomeDiv = document.createElement('div');
            welcomeDiv.className = 'message assistant-message';
            welcomeDiv.id = 'welcomeMsg';
            welcomeDiv.innerHTML = `<div class="bubble">${i18n[currentLang].welcome}</div><div class="message-time">${getTimeStr()}</div>`;
            messagesContainer.appendChild(welcomeDiv);
            conversationHistory = [ { role: 'system', content: '你是一个友好、专业的AI助手，名字叫 DeepSeek，用中文回复，回答简洁有帮助。' } ];
            currentSessionId = 'session_' + Date.now();
        }

        // 添加消息UI
        function addUserMessage(text) {
            const div = document.createElement('div');
            div.className = 'message user-message';
            div.innerHTML = `<div class="bubble">${escapeHtml(text)}</div><div class="message-time">${getTimeStr()}</div>`;
            messagesContainer.appendChild(div);
            scrollToBottom();
        }
        function addAssistantMessage(text, updateHistory = true) {
            const div = document.createElement('div');
            div.className = 'message assistant-message';
            div.innerHTML = `<div class="bubble">${escapeHtml(text)}</div><div class="message-time">${getTimeStr()}</div>`;
            messagesContainer.appendChild(div);
            scrollToBottom();
            if (updateHistory) {
                conversationHistory.push({ role: 'assistant', content: text });
            }
        }
        function addErrorMsg(text) {
            const div = document.createElement('div');
            div.className = 'message assistant-message';
            div.innerHTML = `<div class="bubble error-bubble">⚠️ ${escapeHtml(text)}</div><div class="message-time">${getTimeStr()}</div>`;
            messagesContainer.appendChild(div);
            scrollToBottom();
        }

        // 加载API Key (使用preserve_api)
        async function loadApiKey() {
            if (typeof PreserveAPI !== 'undefined' && PreserveAPI.getKey) {
                const key = await PreserveAPI.getKey();
                if (key) currentApiKey = key;
                else currentApiKey = '';
            } else {
                // fallback
                currentApiKey = localStorage.getItem('deepseek_api_key') || '';
            }
            return currentApiKey;
        }

        async function ensureApiKey() {
            if (currentApiKey) return true;
            await loadApiKey();
            if (!currentApiKey) {
                const inputKey = prompt('请输入 DeepSeek API Key (将保存到 data/api.txt):');
                if (inputKey && inputKey.trim()) {
                    currentApiKey = inputKey.trim();
                    if (typeof PreserveAPI !== 'undefined' && PreserveAPI.saveKey) {
                        await PreserveAPI.saveKey(currentApiKey);
                    } else {
                        localStorage.setItem('deepseek_api_key', currentApiKey);
                    }
                    return true;
                }
                addErrorMsg('未配置API Key，无法发送消息。');
                return false;
            }
            return true;
        }

        // 调用API
        async function callDeepSeek(userMsg) {
            const messages = [...conversationHistory, { role: 'user', content: userMsg }];
            try {
                const controller = new AbortController();
                const timeout = setTimeout(() => controller.abort(), 45000);
                const resp = await fetch('https://api.deepseek.com/v1/chat/completions', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json', 'Authorization': `Bearer ${currentApiKey}` },
                    body: JSON.stringify({ model: 'deepseek-chat', messages, temperature: 0.7, max_tokens: 1024, stream: false }),
                    signal: controller.signal
                });
                clearTimeout(timeout);
                if (!resp.ok) {
                    if (resp.status === 401) { currentApiKey = ''; if(window.PreserveAPI) PreserveAPI.clearKey(); }
                    const err = await resp.json().catch(()=>({}));
                    throw new Error(err.error?.message || `HTTP ${resp.status}`);
                }
                const data = await resp.json();
                return data.choices[0].message.content;
            } catch(e) {
                throw e;
            }
        }

        // 发送消息主流程
        async function handleSend() {
            if (isWaitingReply) return;
            const raw = userInputEl.value.trim();
            if (!raw && !pendingFile) return;
            if (!await ensureApiKey()) return;
            const userMessage = raw || (pendingFile ? `[文件: ${pendingFile.name}]` : ''); 
            // 保存待上传的文件内容
            const fileToSend = pendingFile;
            userInputEl.value = '';
            userInputEl.style.height = 'auto';
            // 显示用户消息（如果有文件，显示文件信息）
            let displayMsg = userMessage;
            if (fileToSend) {
                displayMsg = userMessage + (userMessage ? '\n' : '') + `📎 附件: ${fileToSend.name}`;
            }
            addUserMessage(displayMsg);
            // 构建对话历史
            let messagesForAPI = [...conversationHistory, { role: 'user', content: userMessage }];
            // 如果有文件，需要读取文件内容并通过原生 API 发送
            if (fileToSend && typeof ADNative !== 'undefined' && ADNative.callNative) {
                try {
                    // 读取文件内容（原生返回 base64）
                    const fileContent = await ADNative.callNative('readFile', { path: fileToSend.path });
                    if (fileContent && fileContent.data) {
                        // 对于图片，可以 base64 嵌入；对于文本，可以提取内容
                        if (fileToSend.mimeType && fileToSend.mimeType.startsWith('image/')) {
                            // 图片：在消息中显示
                            addAssistantMessage(`[图片已接收，路径: ${fileToSend.path}]`, false);
                        } else if (fileToSend.mimeType === 'text/plain' || fileToSend.name.endsWith('.txt')) {
                            // 文本文件：提取内容
                            const textContent = atob(fileContent.data);
                            messagesForAPI[messagesForAPI.length - 1].content = 
                                `${userMessage}\n\n文件内容:\n${textContent.substring(0, 3000)}${textContent.length > 3000 ? '...(已截断)' : ''}`;
                        } else {
                            // 其他文件：告知文件名和大小
                            messagesForAPI[messagesForAPI.length - 1].content = 
                                `${userMessage}\n[用户上传了文件: ${fileToSend.name}, 类型: ${fileToSend.mimeType}]`;
                        }
                    }
                } catch(e) {
                    console.error('读取文件失败:', e);
                }
            }

            conversationHistory.push({ role: 'user', content: messagesForAPI[messagesForAPI.length - 1].content });
            // 清除待上传文件
            pendingFile = null;
            filePreview.style.display = 'none';
            // 显示加载指示器...
            const typingDiv = document.createElement('div');
            typingDiv.className = 'message assistant-message';
            typingDiv.id = 'typingIndicator';
            typingDiv.innerHTML = `<div class="typing-indicator" style="display:flex;gap:6px;padding:12px 16px;background:var(--bubble-assistant);border-radius:24px;"><span>●</span><span>●</span><span>●</span> 处理中</div>`;
            messagesContainer.appendChild(typingDiv);
            scrollToBottom();
            isWaitingReply = true;
            try {
                const reply = await callDeepSeekWithMessages(messagesForAPI);
                typingDiv.remove();
                if (reply) addAssistantMessage(reply, true);
                else throw new Error('空回复');
            } catch(e) {
                typingDiv.remove();
                addErrorMsg(`请求失败: ${e.message}`);
            } finally {
                isWaitingReply = false;
            }
        }

        async function callDeepSeekWithMessages(messages) {
            try {
                const controller = new AbortController();
                const timeout = setTimeout(() => controller.abort(), 60000);
                const resp = await fetch('https://api.deepseek.com/v1/chat/completions', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json', 'Authorization': `Bearer ${currentApiKey}` },
                    body: JSON.stringify({ model: 'deepseek-chat', messages, temperature: 0.7, max_tokens: 2048, stream: false }),
                    signal: controller.signal
                });
                clearTimeout(timeout);
                if (!resp.ok) {
                    if (resp.status === 401) { currentApiKey = ''; if(window.PreserveAPI) PreserveAPI.clearKey(); }
                    const err = await resp.json().catch(()=>({}));
                    throw new Error(err.error?.message || `HTTP ${resp.status}`);
                }
                const data = await resp.json();
                return data.choices[0].message.content;
            } catch(e) {
                throw e;
            }
        }
        // 保存历史到 data/history/ (通过 preserve_api)
        async function saveCurrentChat() {
            if (conversationHistory.length <= 1) { alert('没有内容可保存'); return; }
            const sessionData = {
                id: currentSessionId,
                timestamp: new Date().toISOString(),
                messages: conversationHistory.slice(1) // 去掉system
            };
            if (typeof PreserveAPI !== 'undefined' && PreserveAPI.saveHistory) {
                await PreserveAPI.saveHistory(currentSessionId, sessionData);
                alert(`✅ ${i18n[currentLang].saveOk}`);
                loadHistoryList();
            } else {
                alert('保存功能需要 preserve_api.js 支持');
            }
        }

        async function loadHistoryList() {
            if (!window.PreserveAPI || !PreserveAPI.listHistories) return;
            const list = await PreserveAPI.listHistories();
            historySelect.innerHTML = '<option value="">📂 加载历史...</option>';
            list.forEach(item => {
                const opt = document.createElement('option');
                opt.value = item.id;
                opt.textContent = `${item.id} (${new Date(item.timestamp).toLocaleString()})`;
                historySelect.appendChild(opt);
            });
        }

        async function loadChatById(sessionId) {
            if (!sessionId) return;
            if (!window.PreserveAPI || !PreserveAPI.loadHistory) return;
            const data = await PreserveAPI.loadHistory(sessionId);
            if (data && data.messages) {
                messagesContainer.innerHTML = '';
                // 重建UI
                conversationHistory = [{ role: 'system', content: '你是一个友好、专业的AI助手，名字叫 DeepSeek，用中文回复，回答简洁有帮助。' }];
                data.messages.forEach(msg => {
                    if (msg.role === 'user') addUserMessage(msg.content);
                    else if (msg.role === 'assistant') addAssistantMessage(msg.content, true);
                });
                currentSessionId = sessionId;
            }
        }

        // 新建聊天
        function newChat() {
            resetToWelcome();
            currentSessionId = 'session_' + Date.now();
        }

        // 初始化事件 & 加载主题/语言
        async function init() {
            // 主题
            const savedTheme = localStorage.getItem('deepseek_theme') || 'light';
            themeSelect.value = savedTheme;
            applyTheme(savedTheme);
            themeSelect.addEventListener('change', e => applyTheme(e.target.value));

            // 语言
            const savedLang = localStorage.getItem('deepseek_lang') || 'zh';
            languageSelect.value = savedLang;
            applyLanguage(savedLang);
            languageSelect.addEventListener('change', e => { applyLanguage(e.target.value); localStorage.setItem('deepseek_lang', e.target.value); });

            // 设置面板
            settingsToggleBtn.addEventListener('click', ()=> settingsPanel.classList.toggle('show'));
            refreshBtn.addEventListener('click', ()=> location.reload());
            newChatBtn.addEventListener('click', newChat);
            showProtocolBtn.addEventListener('click', ()=> alert(i18n[currentLang].protocol));
            resetApiKeyBtn.addEventListener('click', async ()=>{
                if (confirm('重置API Key？')) {
                    if(window.PreserveAPI) await PreserveAPI.clearKey();
                    currentApiKey = '';
                    await ensureApiKey();
                }
            });

            // 加载key
            await loadApiKey();
            if (!currentApiKey) await ensureApiKey();

            // 欢迎消息
            resetToWelcome();

            // 发送
            sendButton.addEventListener('click', handleSend);
            userInputEl.addEventListener('keydown', e=> { if(e.key==='Enter'&&!e.shiftKey) { e.preventDefault(); handleSend(); } });
            userInputEl.addEventListener('input', ()=> { userInputEl.style.height = 'auto'; userInputEl.style.height = Math.min(userInputEl.scrollHeight,120)+'px'; });

            // 历史相关
            saveHistoryBtn.addEventListener('click', saveCurrentChat);
            historySelect.addEventListener('change', e=> { if(e.target.value) loadChatById(e.target.value); });
            // 初始化历史列表
            if (window.PreserveAPI && PreserveAPI.init) await PreserveAPI.init();
            await loadHistoryList();

            // 自动调整
            window.addEventListener('load', ()=> scrollToBottom());
        }

        init();
    })();
</script>
<!-- 额外的打字指示器样式 -->
<style>
    .typing-indicator span { margin:0 2px; }
    .btn-small { background: var(--input-bg); border:1px solid var(--border-light); padding:6px 14px; border-radius:30px; cursor:pointer; }
</style>
</body>
</html>
```

```www/preserve_api.js
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
```


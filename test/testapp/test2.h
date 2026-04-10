// test2.h
# include <string>
std::string html_text = R"(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
    <title>test deepseek ai</title>
    <link rel="stylesheet" href="css/style.css">
    <style>
        * {
            box-sizing: border-box;
        }

        body {
            font-family: system-ui, -apple-system, 'Segoe UI', Roboto, 'Helvetica Neue', sans-serif;
            background: #f7f9fc;
            margin: 0;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .chat-app {
            max-width: 800px;
            width: 100%;
            background: white;
            border-radius: 28px;
            box-shadow: 0 20px 35px -12px rgba(0,0,0,0.1);
            overflow: hidden;
            transition: all 0.2s ease;
        }
        .chat-header {
            background: #ffffff;
            padding: 18px 24px;
            border-bottom: 1px solid #e9edf2;
            display: flex;
            align-items: center;
            gap: 12px;
        }
        .chat-header h1 {
            margin: 0;
            font-size: 1.5rem;
            font-weight: 600;
            background: linear-gradient(135deg, #0e6dfd, #0a58ca);
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
        .messages-area {
            height: 460px;
            overflow-y: auto;
            padding: 20px 20px 10px 20px;
            background: #fefefe;
            display: flex;
            flex-direction: column;
            gap: 16px;
            scroll-behavior: smooth;
        }
        .message {
            display: flex;
            flex-direction: column;
            max-width: 85%;
            animation: fadeInUp 0.25s ease-out;
        }
        .user-message {
            align-self: flex-end;
        }
        .assistant-message {
            align-self: flex-start;
        }
        .bubble {
            padding: 12px 16px;
            border-radius: 24px;
            line-height: 1.45;
            font-size: 0.95rem;
            word-break: break-word;
            white-space: pre-wrap;
            box-shadow: 0 1px 1px rgba(0,0,0,0.05);
        }
        .user-message .bubble {
            background: #0a66e9;
            color: white;
            border-bottom-right-radius: 6px;
        }
        .assistant-message .bubble {
            background: #f0f2f5;
            color: #1e2a3e;
            border-bottom-left-radius: 6px;
        }
        .message-time {
            font-size: 0.7rem;
            margin-top: 6px;
            margin-left: 12px;
            margin-right: 12px;
            color: #8e9aaf;
        }
        .user-message .message-time {
            text-align: right;
        }
        .input-area {
            padding: 16px 20px 20px;
            background: white;
            border-top: 1px solid #edf2f7;
            display: flex;
            gap: 12px;
            align-items: flex-end;
        }
        .input-wrapper {
            flex: 1;
            background: #f8fafc;
            border-radius: 32px;
            border: 1px solid #e2e8f0;
            transition: 0.2s;
            display: flex;
            align-items: center;
            padding: 6px 16px;
        }
        .input-wrapper:focus-within {
            border-color: #0a66e9;
            box-shadow: 0 0 0 3px rgba(10,102,233,0.15);
            background: white;
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
            max-height: 120px;
        }
        #sendBtn {
            background: #0a66e9;
            border: none;
            color: white;
            width: 44px;
            height: 44px;
            border-radius: 44px;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            transition: 0.2s;
            box-shadow: 0 2px 6px rgba(0,0,0,0.1);
        }
        #sendBtn:hover {
            background: #0056c2;
            transform: scale(0.96);
        }
        #sendBtn:active {
            background: #0043a0;
        }
        .send-icon {
            font-size: 1.4rem;
            line-height: 1;
        }
        .typing-indicator {
            display: flex;
            align-items: center;
            gap: 6px;
            background: #f0f2f5;
            padding: 10px 16px;
            border-radius: 24px;
            width: fit-content;
        }
        .typing-indicator span {
            width: 8px;
            height: 8px;
            background: #7c8ba0;
            border-radius: 50%;
            display: inline-block;
            animation: blink 1.4s infinite ease both;
        }
        .typing-indicator span:nth-child(2) { animation-delay: 0.2s; }
        .typing-indicator span:nth-child(3) { animation-delay: 0.4s; }
        @keyframes blink {
            0%, 80%, 100% { opacity: 0.3; transform: scale(0.8);}
            40% { opacity: 1; transform: scale(1);}
        }
        @keyframes fadeInUp {
            from {
                opacity: 0;
                transform: translateY(12px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        @media (max-width: 560px) {
            .message {
                max-width: 92%;
            }
            .chat-app {
                border-radius: 20px;
            }
            .messages-area {
                height: 420px;
            }
        }
        .messages-area::-webkit-scrollbar {
            width: 5px;
        }
        .messages-area::-webkit-scrollbar-track {
            background: #eef2f9;
        }
        .messages-area::-webkit-scrollbar-thumb {
            background: #b9c4d4;
            border-radius: 10px;
        }
        .error-bubble {
            background: #ffe6e5 !important;
            color: #c62828 !important;
        }
    </style>
</head>
<body>
<div class="chat-app">
    <div class="chat-header">
        <h1>DeepSeek</h1>
    </div>
    <div class="messages-area" id="messagesArea">
        <div class="message assistant-message" id="welcomeMsg">
            <div class="bubble">
                你好!我是 DeepSeek , 有什么可以帮你的吗?直接输入消息,我会尽快回复你.
            </div>
            <div class="message-time">刚刚</div>
        </div>
    </div>
    <div class="input-area">
        <div class="input-wrapper">
            <textarea id="userInput" placeholder="输入你的问题..." rows="1" style="overflow-y: auto;"></textarea>
        </div>
        <button id="sendBtn" aria-label="发送消息">
            <span class="send-icon">➤</span>
        </button>
    </div>
</div>

<script>
    // DOM 元素
    const messagesContainer = document.getElementById('messagesArea');
    const userInputEl = document.getElementById('userInput');
    const sendButton = document.getElementById('sendBtn');

    let conversationHistory = [
        { role: 'system', content: '你是一个友好、专业的AI助手，名字叫DeepSeek，用中文回复，回答简洁有帮助。' }
    ];

    // 是否正在等待回复 (防止重复发送)
    let isWaitingReply = false;

    let apiKey = '';

    // 初始化 API Key
    function initApiKey() {
        const storedKey = localStorage.getItem('deepseek_api_key');
        if (storedKey && storedKey.trim() !== '') {
            apiKey = storedKey.trim();
            return true;
        } else {
            const userKey = prompt('请提供你的 DeepSeek API Key 以使用 AI 功能\n(获取地址: https://platform.deepseek.com/api_keys)\n\n输入后会自动保存至本地存储，下次无需重新输入。');
            if (userKey && userKey.trim() !== '') {
                apiKey = userKey.trim();
                localStorage.setItem('deepseek_api_key', apiKey);
                return true;
            } else {
                addSystemErrorMessage('未配置 API Key，请刷新页面后重新输入有效的 DeepSeek API Key。');
                return false;
            }
        }
    }

    function addSystemErrorMessage(text) {
        const errorMsgDiv = document.createElement('div');
        errorMsgDiv.className = 'message assistant-message';
        const timeStr = new Date().toLocaleTimeString([], { hour:'2-digit', minute:'2-digit' });
        errorMsgDiv.innerHTML = `
            <div class="bubble error-bubble"> ${escapeHtml(text)}</div>
            <div class="message-time">${timeStr}</div>
        `;
        messagesContainer.appendChild(errorMsgDiv);
        scrollToBottom();
    }

    // 辅助函数: 转义 HTML 防止 XSS
    function escapeHtml(str) {
        if (!str) return '';
        return str.replace(/[&<>]/g, function(m) {
            if (m === '&') return '&amp;';
            if (m === '<') return '&lt;';
            if (m === '>') return '&gt;';
            return m;
        }).replace(/[\uD800-\uDBFF][\uDC00-\uDFFF]/g, function(c) {
            return c;
        });
    }

    // 滚动到底部
    function scrollToBottom() {
        messagesContainer.scrollTop = messagesContainer.scrollHeight;
    }

    let typingIndicatorElem = null;
    function showTypingIndicator() {
        removeTypingIndicator();
        const indicatorDiv = document.createElement('div');
        indicatorDiv.className = 'message assistant-message';
        indicatorDiv.id = 'typingIndicator';
        indicatorDiv.innerHTML = `
            <div class="typing-indicator">
                <span></span><span></span><span></span>
                <span style="margin-left: 6px; font-size:0.8rem;">AI 正在思考...</span>
            </div>
        `;
        messagesContainer.appendChild(indicatorDiv);
        scrollToBottom();
        typingIndicatorElem = indicatorDiv;
    }

    function removeTypingIndicator() {
        if (typingIndicatorElem && typingIndicatorElem.parentNode) {
            typingIndicatorElem.remove();
            typingIndicatorElem = null;
        } else {
            const existing = document.getElementById('typingIndicator');
            if (existing) existing.remove();
        }
    }

    // 添加用户消息到界面和历史
    function addUserMessageToUI(text) {
        const timeStr = new Date().toLocaleTimeString([], { hour:'2-digit', minute:'2-digit' });
        const messageDiv = document.createElement('div');
        messageDiv.className = 'message user-message';
        messageDiv.innerHTML = `
            <div class="bubble">${escapeHtml(text)}</div>
            <div class="message-time">${timeStr}</div>
        `;
        messagesContainer.appendChild(messageDiv);
        scrollToBottom();
    }

    // 添加 AI 消息到界面和历史
    function addAssistantMessageToUI(text, updateHistory = true) {
        const timeStr = new Date().toLocaleTimeString([], { hour:'2-digit', minute:'2-digit' });
        const messageDiv = document.createElement('div');
        messageDiv.className = 'message assistant-message';
        messageDiv.innerHTML = `
            <div class="bubble">${escapeHtml(text)}</div>
            <div class="message-time">${timeStr}</div>
        `;
        messagesContainer.appendChild(messageDiv);
        scrollToBottom();

        if (updateHistory) {
            conversationHistory.push({ role: 'assistant', content: text });
            // 控制历史长度 (可选保留最近20组)
            if (conversationHistory.length > 30) {
                // 保留 system 消息和最近的消息
                const systemMsg = conversationHistory[0];
                const newHistory = [systemMsg, ...conversationHistory.slice(-24)];
                conversationHistory = newHistory;
            }
        }
    }

    // 核心调用 DeepSeek API (基于原有 ai.js 结构增强)
    async function sendMessageToAI(userMessage) {
        if (!apiKey) {
            // 尝试重新获取
            const reInit = initApiKey();
            if (!reInit || !apiKey) {
                addSystemErrorMessage('API Key 无效或缺失，请刷新页面并输入有效 Key。');
                isWaitingReply = false;
                removeTypingIndicator();
                return null;
            }
        }

        // 构建请求消息体 (包含历史)
        const requestMessages = [
            ...conversationHistory,
            { role: 'user', content: userMessage }
        ];

        try {
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), 45000); // 45秒超时

            const response = await fetch('https://api.deepseek.com/v1/chat/completions', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${apiKey}`
                },
                body: JSON.stringify({
                    model: 'deepseek-chat',
                    messages: requestMessages,
                    temperature: 0.7,
                    max_tokens: 1024,
                    stream: false
                }),
                signal: controller.signal
            });

            clearTimeout(timeoutId);

            if (!response.ok) {
                let errorDetail = `API 请求失败 (${response.status})`;
                if (response.status === 401) {
                    errorDetail = '认证失败: API Key 无效或已过期，请检查密钥。';
                    // 清除存储的无效 key
                    localStorage.removeItem('deepseek_api_key');
                    apiKey = '';
                    initApiKey(); // 重新提示输入
                } else if (response.status === 429) {
                    errorDetail = '请求过于频繁，请稍后再试。';
                } else {
                    try {
                        const errData = await response.json();
                        errorDetail = errData.error?.message || errorDetail;
                    } catch(e) {}
                }
                throw new Error(errorDetail);
            }

            const data = await response.json();
            if (data && data.choices && data.choices.length > 0) {
                const aiReply = data.choices[0].message.content;
                return aiReply;
            } else {
                throw new Error('API 返回数据格式异常');
            }
        } catch (error) {
            console.error('DeepSeek API 调用失败:', error);
            let userFriendlyMsg = `网络或服务异常: ${error.message}`;
            if (error.name === 'AbortError') userFriendlyMsg = '请求超时，请检查网络后重试。';
            addSystemErrorMessage(userFriendlyMsg);
            return null;
        }
    }

    // 主要发送流程: 处理用户输入, 显示UI, 调用API, 展示回复
    async function handleSendMessage() {
        if (isWaitingReply) {
            addSystemErrorMessage('请等待上一个回复完成后再发送新消息~');
            return;
        }
        let rawMessage = userInputEl.value.trim();
        if (rawMessage === '') {
            // 轻提醒但不报错
            userInputEl.style.border = '1px solid #ffaaa5';
            setTimeout(() => {
                userInputEl.style.border = '';
            }, 600);
            return;
        }

        // 清空输入框并重置高度
        userInputEl.value = '';
        userInputEl.style.height = 'auto';

        // 添加用户消息到界面
        addUserMessageToUI(rawMessage);
        // 存入对话历史 (用户消息)
        conversationHistory.push({ role: 'user', content: rawMessage });

        // 开始等待AI回复标志
        isWaitingReply = true;
        // 显示输入指示器
        showTypingIndicator();

        // 调用 AI 接口
        const aiReplyText = await sendMessageToAI(rawMessage);

        // 移除指示器
        removeTypingIndicator();

        if (aiReplyText && typeof aiReplyText === 'string') {
            // 显示AI回复到界面, 并自动加入历史 (addAssistantMessageToUI 会负责将回复推入conversationHistory)
            addAssistantMessageToUI(aiReplyText, true);
        } else if (aiReplyText === null) {

            if (conversationHistory.length > 0 && conversationHistory[conversationHistory.length-1].role === 'user') {
                conversationHistory.pop();
            }

            const errorHintDiv = document.createElement('div');
            errorHintDiv.className = 'message assistant-message';
            errorHintDiv.innerHTML = `<div class="bubble error-bubble">抱歉，AI 暂时无法响应，请检查API Key或稍后再试。</div><div class="message-time">${new Date().toLocaleTimeString([], {hour:'2-digit', minute:'2-digit'})}</div>`;
            messagesContainer.appendChild(errorHintDiv);
            scrollToBottom();
        }

        // 重置等待标志
        isWaitingReply = false;

        // 滚动到底部确保看到最新消息
        scrollToBottom();
    }

    // 自动调整 textarea 高度
    function autoResizeTextarea() {
        userInputEl.style.height = 'auto';
        const newHeight = Math.min(userInputEl.scrollHeight, 120);
        userInputEl.style.height = newHeight + 'px';
    }

    // 事件绑定
    sendButton.addEventListener('click', () => {
        handleSendMessage();
    });

    userInputEl.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' && !e.shiftKey) {
            e.preventDefault();
            handleSendMessage();
        }
    });
    userInputEl.addEventListener('input', autoResizeTextarea);

    window.addEventListener('DOMContentLoaded', () => {
        // 先尝试获取 Key
        const keyStatus = initApiKey();
        if (!keyStatus) {
            // 未配置key, 显示静态但允许后续刷新重试
            const warnMsg = document.createElement('div');
            warnMsg.className = 'message assistant-message';
            warnMsg.innerHTML = `<div class="bubble error-bubble">未检测到 API Key，请在弹窗中输入有效的 DeepSeek Key 以开始对话。</div><div class="message-time">${new Date().toLocaleTimeString([], {hour:'2-digit', minute:'2-digit'})}</div>`;
            messagesContainer.appendChild(warnMsg);
            scrollToBottom();
        } else {
            console.log('API Key 已配置，可以开始对话');
            // 展示一个小小状态, 可选
        }

        const existingWelcome = document.getElementById('welcomeMsg');
        if (existingWelcome) {
            // 可以保留, 但在 conversationHistory 中无影响，挺好的
        }
    });

    // 暴露一个清理 Key 的辅助 (方便调试，但不必须)
    window.clearApiKeyForTest = () => {
        localStorage.removeItem('deepseek_api_key');
        apiKey = '';
        location.reload();
    };
</script>
</body>
</html>
)";

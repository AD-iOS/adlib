// ADlibc++-objc-testapi.hpp

# ifndef _ADLIBCXX_OBJC_TESTAPI_HPP_
# define _ADLIBCXX_OBJC_TESTAPI_HPP_

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"

# if defined(__OBJC__) && defined(__APPLE__)
# import <Foundation/Foundation.h>
# import <WebKit/WebKit.h>
# import <UIKit/UIKit.h>
# import <objc/runtime.h>
# define ad_read
# import <AD/ADlibc++.hpp>

static const void *ADWebViewBridgeKey = &ADWebViewBridgeKey; /* 不能添加 _Nonnull 除非你想要重構這個文件 */
# else
    # error "only apple platform and use objc/objc++ code, only then can it be used"
# endif // __OBJC__ && __APPLE__

# ifdef ad_objc_use_scheme
@interface ADLocalSchemeHandler : NSObject <WKURLSchemeHandler>
@property (nonatomic, strong) NSString *basePath;
- (instancetype)initWithBasePath:(NSString *)basePath;
@end

@implementation ADLocalSchemeHandler
- (instancetype)initWithBasePath:(NSString *)basePath {
    self = [super init];
    if (self) { 
        _basePath = basePath;
    }
    return self;
}

- (void)webView:(WKWebView *)webView startURLSchemeTask:(id<WKURLSchemeTask>)urlSchemeTask {
    NSURL *url = urlSchemeTask.request.URL;
    NSString *path = url.path;
    if (path.length == 0 || [path isEqualToString:@"/"]) {
        path = @"/index.html";
    }

    NSString *decodedPath = [path stringByRemovingPercentEncoding];
    if (!decodedPath) decodedPath = path;

    NSString *filePath = [self.basePath stringByAppendingPathComponent:decodedPath];

    if (![filePath hasPrefix:self.basePath]) {
        NSError *error = [NSError errorWithDomain:@"ADLocalSchemeError" 
                                             code:403 
                                         userInfo:@{NSLocalizedDescriptionKey: @"Access denied"}];
        [urlSchemeTask didFailWithError:error];
        return;
    }

    NSFileManager *fm = [NSFileManager defaultManager];
    BOOL isDirectory;
    if (![fm fileExistsAtPath:filePath isDirectory:&isDirectory] || isDirectory) {
        if (isDirectory) {
            filePath = [filePath stringByAppendingPathComponent:@"index.html"];
            if (![fm fileExistsAtPath:filePath]) {
                [self send404:urlSchemeTask url:url];
                return;
            }
        } else {
            [self send404:urlSchemeTask url:url];
            return;
        }
    }

    NSData *data = [NSData dataWithContentsOfFile:filePath];
    if (!data) {
        [self send404:urlSchemeTask url:url];
        return;
    }

    NSString *mimeType = [self mimeTypeForPath:filePath];
    
    NSURLResponse *response = [[NSURLResponse alloc] initWithURL:url 
                                                        MIMEType:mimeType 
                                           expectedContentLength:data.length 
                                                textEncodingName:@"utf-8"];
    [urlSchemeTask didReceiveResponse:response];
    [urlSchemeTask didReceiveData:data];
    [urlSchemeTask didFinish];
}

- (void)send404:(id<WKURLSchemeTask>)urlSchemeTask url:(NSURL *)url {
    NSLog(@"[ADLocalScheme] File Not Found: %@", url.path);
    NSError *error = [NSError errorWithDomain:@"ADLocalSchemeError" 
                                         code:404 
                                     userInfo:@{NSLocalizedDescriptionKey: @"File not found"}];
    [urlSchemeTask didFailWithError:error];
}

- (NSString *)mimeTypeForPath:(NSString *)filePath {
    NSString *extension = [filePath pathExtension].lowercaseString;
    NSDictionary *mimeTypes = @{
        @"html": @"text/html",
        @"htm": @"text/html",
        @"css": @"text/css",
        @"js": @"text/javascript",
        @"json": @"application/json",
        @"png": @"image/png",
        @"jpg": @"image/jpeg",
        @"jpeg": @"image/jpeg",
        @"gif": @"image/gif",
        @"svg": @"image/svg+xml",
        @"webp": @"image/webp",
        @"ico": @"image/x-icon",
        @"txt": @"text/plain",
        @"xml": @"application/xml",
        @"pdf": @"application/pdf",
        @"zip": @"application/zip"
    };
    NSString *mime = mimeTypes[extension];
    return mime ?: @"application/octet-stream";
}

- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id<WKURLSchemeTask>)urlSchemeTask {
    // 不需要特殊处理
}
@end
# endif // ad_objc_use_scheme

#ifdef ad_objc_use_web
// UIWebView 桥接类: 处理 JS 调用原生
@interface ADWebViewBridge : NSObject <UIWebViewDelegate, UIDocumentPickerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@property (nonatomic, weak) UIWebView *webView;
@property (nonatomic, copy) void (^messageHandler)(NSString *name, id body);
@property (nonatomic, copy) NSString *bridgeName;
- (instancetype)initWithWebView:(UIWebView *)webView 
                   bridgeName:(NSString *)bridgeName
                messageHandler:(void (^)(NSString *name, id body))handler;
- (void)injectJSBridge;
- (void)sendCallbackToJS:(NSString *)callbackId result:(id)result error:(NSString *)error;
@end

@implementation ADWebViewBridge
- (instancetype)initWithWebView:(UIWebView *)webView 
                   bridgeName:(NSString *)bridgeName
                messageHandler:(void (^)(NSString *name, id body))handler {
    self = [super init];
    if (self) {
        _webView = webView;
        _bridgeName = bridgeName;
        _messageHandler = handler;
        webView.delegate = self;
    }
    return self;
}

- (void)injectJSBridge {
    NSString *js = [NSString stringWithFormat:
        @"window.%@ = {"
        "    postMessage: function(data) {"
        "        var iframe = document.createElement('iframe');"
        "        iframe.style.display = 'none';"
        "        iframe.src = 'adbridge://call?' + encodeURIComponent(JSON.stringify(data));"
        "        document.body.appendChild(iframe);"
        "        setTimeout(function() { iframe.remove(); }, 0);"
        "    },"
        "    callNative: function(method, params) {"
        "        return new Promise(function(resolve, reject) {"
        "            var callbackId = Date.now() + '_' + Math.random();"
        "            window._adCallbacks = window._adCallbacks || {};"
        "            window._adCallbacks[callbackId] = { resolve: resolve, reject: reject };"
        "            window.%@.postMessage({ method: method, params: params, callbackId: callbackId });"
        "        });"
        "    }"
        "};"
        "window._nativeCallback = function(callbackId, result, error) {"
        "    var cb = window._adCallbacks[callbackId];"
        "    if (cb) {"
        "        if (error) { cb.reject(new Error(error)); }"
        "        else { cb.resolve(result); }"
        "        delete window._adCallbacks[callbackId];"
        "    }"
        "};",
        _bridgeName, _bridgeName
    ];
    [_webView stringByEvaluatingJavaScriptFromString:js];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSURL *url = request.URL;
    // 拦截自定义协议
    if ([url.scheme isEqualToString:@"adbridge"]) {
        if ([url.host isEqualToString:@"call"]) {
            NSString *jsonString = [url.query stringByRemovingPercentEncoding];
            if (jsonString) {
                NSData *data = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
                NSError *error;
                id body = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
                if (self.messageHandler) {
                    self.messageHandler(self.bridgeName, body);
                }
            }
        }
        return NO;
    }
    // 调用外部 delegate(如果有)
    if ([self respondsToSelector:@selector(webView:shouldStartLoadWithRequest:navigationType:)]) {
        // 可以转发给外部 delegate, TODO:
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    [self injectJSBridge]; // 确保每次加载完成都重新注入
}

// - (void)sendCallbackToJS:(NSString *)callbackId result:(id)result error:(NSString *)error;

- (void)sendCallbackToJS:(NSString *)callbackId result:(id)result error:(NSString *)error {
    NSString *js;
    if (error) {
        js = [NSString stringWithFormat:@"window._nativeCallback('%@', null, '%@');", 
              callbackId, [error stringByReplacingOccurrencesOfString:@"'" withString:@"\\'"]];
    } else {
        NSData *data = [NSJSONSerialization dataWithJSONObject:result options:0 error:nil];
        NSString *resultStr = data ? [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] : @"null";
        js = [NSString stringWithFormat:@"window._nativeCallback('%@', %@, null);", callbackId, resultStr];
    }
    [_webView stringByEvaluatingJavaScriptFromString:js];
}

# pragma mark - 文件选择相关

// 获取当前最顶层的 ViewController
- (UIViewController *)getTopViewController {
    UIViewController *topVC = [UIApplication sharedApplication].keyWindow.rootViewController;
    while (topVC.presentedViewController) {
        topVC = topVC.presentedViewController;
    }
    return topVC;
}

// 显示文件选择器
- (void)showDocumentPickerWithCallbackId:(NSString *)callbackId {
    // 支持的文档类型
    NSArray *types = @[
        @"public.text",      // txt, 代码文件
        @"public.image",     // 图片
        @"public.pdf",       // PDF
        @"public.json",      // JSON
        @"public.xml",       // XML
        @"com.apple.keynote",// Keynote
        @"org.openxmlformats.wordprocessingml.document", // docx
        @"org.openxmlformats.presentationml.presentation", // pptx
        @"org.openxmlformats.spreadsheetml.sheet", // xlsx
        @"com.microsoft.word.doc",
        @"com.microsoft.powerpoint.ppt",
        @"com.microsoft.excel.xls"
    ];
    
    UIDocumentPickerViewController *picker = [[UIDocumentPickerViewController alloc] 
        initWithDocumentTypes:types inMode:UIDocumentPickerModeImport];
    picker.delegate = self;
    picker.modalPresentationStyle = UIModalPresentationFormSheet;
    
    // 存储 callbackId
    objc_setAssociatedObject(picker, "callbackId", callbackId, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    objc_setAssociatedObject(picker, "bridge", self, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    
    [[self getTopViewController] presentViewController:picker animated:YES completion:nil];
}

// 显示图片选择器（拍照/相册）
- (void)showImagePickerWithSourceType:(UIImagePickerControllerSourceType)type callbackId:(NSString *)callbackId {
    if (![UIImagePickerController isSourceTypeAvailable:type]) {
        [self sendCallbackToJS:callbackId result:nil error:@"Source not available"];
        return;
    }
    
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.sourceType = type;
    picker.delegate = self;
    picker.allowsEditing = NO;
    
    objc_setAssociatedObject(picker, "callbackId", callbackId, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    
    [[self getTopViewController] presentViewController:picker animated:YES completion:nil];
}

- (NSString *)getUploadDirectory {
    // 获取 www 目录路径
    NSString *wwwPath = nil;
    
    // 方法1: 通过 webView 的 baseURL 获取（如果已加载）
    if (self.webView && self.webView.request.URL) {
        NSURL *baseURL = self.webView.request.URL;
        if ([baseURL.scheme isEqualToString:@"file"]) {
            wwwPath = [[baseURL.path stringByDeletingLastPathComponent] stringByDeletingLastPathComponent];
        }
    }
    
    // 方法2: 使用默认路径（你需要根据实际情况调整）
    if (!wwwPath) {
        // 获取应用沙盒 Documents 目录
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [paths firstObject];
        wwwPath = [documentsPath stringByAppendingPathComponent:@"www"];
    }
    
    NSString *uploadDir = [wwwPath stringByAppendingPathComponent:@"data/uploadfile"];
    
    // 创建目录
    NSFileManager *fm = [NSFileManager defaultManager];
    if (![fm fileExistsAtPath:uploadDir]) {
        [fm createDirectoryAtPath:uploadDir withIntermediateDirectories:YES attributes:nil error:nil];
    }
    
    return uploadDir;
}

// 保存选中的文件到临时目录并返回信息
- (NSDictionary *)saveFileToUploadDir:(NSURL *)fileURL {
    // 获取文件访问权限
    BOOL gotAccess = [fileURL startAccessingSecurityScopedResource];
    
    NSString *uploadDir = [self getUploadDirectory];
    NSString *destPath = [uploadDir stringByAppendingPathComponent:fileURL.lastPathComponent];
    
    // 如果文件已存在，添加时间戳
    NSFileManager *fm = [NSFileManager defaultManager];
    if ([fm fileExistsAtPath:destPath]) {
        NSString *nameWithoutExt = [fileURL.lastPathComponent stringByDeletingPathExtension];
        NSString *ext = [fileURL.pathExtension length] ? [@"." stringByAppendingString:fileURL.pathExtension] : @"";
        destPath = [uploadDir stringByAppendingPathComponent:
            [NSString stringWithFormat:@"%@_%.0f%@", nameWithoutExt, [[NSDate date] timeIntervalSince1970], ext]];
    }
    
    NSError *error = nil;
    [fm copyItemAtURL:fileURL toURL:[NSURL fileURLWithPath:destPath] error:&error];
    
    if (gotAccess) {
        [fileURL stopAccessingSecurityScopedResource];
    }
    
    if (error) {
        return nil;
    }
    
    NSDictionary *attrs = [fm attributesOfItemAtPath:destPath error:nil];
    NSString *mimeType = [self mimeTypeForPath:destPath];
    
    return @{
        @"name": fileURL.lastPathComponent,
        @"path": destPath,
        @"size": @([attrs fileSize]),
        @"mimeType": mimeType ?: @"application/octet-stream"
    };
}

- (NSDictionary *)saveImageToUploadDir:(UIImage *)image {
    NSString *uploadDir = [self getUploadDirectory];
    NSString *imagePath = [uploadDir stringByAppendingPathComponent:
        [NSString stringWithFormat:@"image_%.0f.jpg", [[NSDate date] timeIntervalSince1970]]];
    
    NSData *imageData = UIImageJPEGRepresentation(image, 0.8);
    [imageData writeToFile:imagePath atomically:YES];
    
    return @{
        @"name": [imagePath lastPathComponent],
        @"path": imagePath,
        @"size": @(imageData.length),
        @"mimeType": @"image/jpeg"
    };
}

- (NSString *)mimeTypeForPath:(NSString *)filePath {
    NSString *ext = [filePath pathExtension].lowercaseString;
    NSDictionary *map = @{
        @"txt": @"text/plain", @"html": @"text/html", @"htm": @"text/html",
        @"css": @"text/css", @"js": @"text/javascript", @"json": @"application/json",
        @"xml": @"application/xml", @"pdf": @"application/pdf",
        @"jpg": @"image/jpeg", @"jpeg": @"image/jpeg", @"png": @"image/png",
        @"gif": @"image/gif", @"svg": @"image/svg+xml", @"webp": @"image/webp",
        @"doc": @"application/msword", @"docx": @"application/vnd.openxmlformats-officedocument.wordprocessingml.document",
        @"xls": @"application/vnd.ms-excel", @"xlsx": @"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
        @"ppt": @"application/vnd.ms-powerpoint", @"pptx": @"application/vnd.openxmlformats-officedocument.presentationml.presentation"
    };
    return map[ext] ?: @"application/octet-stream";
}

# pragma mark - UIDocumentPickerDelegate

- (void)documentPicker:(UIDocumentPickerViewController *)controller didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
    NSString *callbackId = objc_getAssociatedObject(controller, "callbackId");
    NSURL *selectedURL = urls.firstObject;
    
    if (!selectedURL) {
        [self sendCallbackToJS:callbackId result:nil error:@"No file selected"];
        return;
    }
    
    // 改为保存到 uploadfile 目录
    NSDictionary *fileInfo = [self saveFileToUploadDir:selectedURL];
    if (fileInfo) {
        [self sendCallbackToJS:callbackId result:@{@"success": @YES, @"fileInfo": fileInfo} error:nil];
    } else {
        [self sendCallbackToJS:callbackId result:nil error:@"Failed to save file"];
    }
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey,id> *)info {
    NSString *callbackId = objc_getAssociatedObject(picker, "callbackId");
    
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    if (!image) {
        [picker dismissViewControllerAnimated:YES completion:nil];
        [self sendCallbackToJS:callbackId result:nil error:@"No image selected"];
        return;
    }
    
    // 改为保存到 uploadfile 目录
    NSDictionary *fileInfo = [self saveImageToUploadDir:image];
    
    [picker dismissViewControllerAnimated:YES completion:^{
        [self sendCallbackToJS:callbackId result:@{@"success": @YES, @"fileInfo": fileInfo} error:nil];
    }];
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller {
    NSString *callbackId = objc_getAssociatedObject(controller, "callbackId");
    [self sendCallbackToJS:callbackId result:nil error:@"cancelled"];
}

# pragma mark - UIImagePickerControllerDelegate
/*
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey,id> *)info {
    NSString *callbackId = objc_getAssociatedObject(picker, "callbackId");
    
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    if (!image) {
        [picker dismissViewControllerAnimated:YES completion:nil];
        [self sendCallbackToJS:callbackId result:nil error:@"No image selected"];
        return;
    }
    
    // 保存图片
    NSString *tempDir = NSTemporaryDirectory();
    NSString *imagePath = [tempDir stringByAppendingPathComponent:
        [NSString stringWithFormat:@"image_%.0f.jpg", [[NSDate date] timeIntervalSince1970]]];
    
    NSData *imageData = UIImageJPEGRepresentation(image, 0.8);
    [imageData writeToFile:imagePath atomically:YES];
    
    NSDictionary *fileInfo = @{
        @"name": [imagePath lastPathComponent],
        @"path": imagePath,
        @"size": @(imageData.length),
        @"mimeType": @"image/jpeg"
    };
    
    [picker dismissViewControllerAnimated:YES completion:^{
        [self sendCallbackToJS:callbackId result:@{@"success": @YES, @"fileInfo": fileInfo} error:nil];
    }];
}
*/

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    NSString *callbackId = objc_getAssociatedObject(picker, "callbackId");
    [picker dismissViewControllerAnimated:YES completion:^{
        [self sendCallbackToJS:callbackId result:nil error:@"cancelled"];
    }];
}

# pragma mark - 读取文件内容

- (void)readFileContent:(NSString *)path callbackId:(NSString *)callbackId {
    NSFileManager *fm = [NSFileManager defaultManager];
    if (![fm fileExistsAtPath:path]) {
        [self sendCallbackToJS:callbackId result:nil error:@"File not found"];
        return;
    }
    
    NSData *data = [NSData dataWithContentsOfFile:path];
    if (!data) {
        [self sendCallbackToJS:callbackId result:nil error:@"Failed to read file"];
        return;
    }
    
    NSString *base64 = [data base64EncodedStringWithOptions:0];
    [self sendCallbackToJS:callbackId result:@{
        @"data": base64,
        @"size": @(data.length)
    } error:nil];
}

@end
// 原生调用 JS 的方法
inline void callJS(UIWebView* webView, const char* script, void (^callback)(NSString *result)) {
    NSString *js = [NSString stringWithUTF8String:script];
    NSString *result = [webView stringByEvaluatingJavaScriptFromString:js];
    if (callback) {
        callback(result);
    }
}
# endif // ad_objc_use_web

namespace AD {
  # if defined(__OBJC__) && defined(__APPLE__) && defined(ad_use_objc_code) || defined(ad_use_objc)
    namespace ui {
        namespace web {
          # ifdef ad_objc_use_web
            // 加载 HTML 字符串
            inline id loadtext(UIView* container, const char* htmlString) {
                UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];
                webView.translatesAutoresizingMaskIntoConstraints = NO;
                [container addSubview:webView];
                [NSLayoutConstraint activateConstraints:@[
                    [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                    [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                    [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                    [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                ]];
                ADWebViewBridge *bridge = [[ADWebViewBridge alloc] initWithWebView:webView
                                                                       bridgeName:@"ADNative"
                                                                    messageHandler:nil];
                objc_setAssociatedObject(webView, ADWebViewBridgeKey, bridge, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
                NSString *html = [NSString stringWithUTF8String:htmlString];
                [webView loadHTMLString:html baseURL:nil];
                return bridge;
            }
/*
            // 配置支持交互
            inline id loadhtml(UIView* container, const char* htmlPath,
                               void (^messageHandler)(NSString *name, id body)) {
                UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];
                webView.scalesPageToFit = YES;
                webView.translatesAutoresizingMaskIntoConstraints = NO;
                [container addSubview:webView];
                [NSLayoutConstraint activateConstraints:@[
                  [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                  [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                  [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                  [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                ]];
                // 创建桥接器
                ADWebViewBridge *bridge = [[ADWebViewBridge alloc] initWithWebView:webView
                                                                         bridgeName:@"ADNative"
                                                                      messageHandler:messageHandler];
                objc_setAssociatedObject(webView, ADWebViewBridgeKey, bridge, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
                // 加载 HTML
                NSString *path = [NSString stringWithUTF8String:htmlPath];
                NSURL *htmlURL = [NSURL fileURLWithPath:path];
                if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
                    [webView loadRequest:[NSURLRequest requestWithURL:htmlURL]];
                } else {
                    [webView loadHTMLString:@"<h1>File Not Found</h1>" baseURL:nil];
                }
                return bridge;
            }
*/
/*
            inline id loadhtml(UIView* container, const char* htmlPath,
                               void (^messageHandler)(NSString *name, id body)) {
                UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];
                webView.scalesPageToFit = YES;
                webView.translatesAutoresizingMaskIntoConstraints = NO;
                [container addSubview:webView];
                [NSLayoutConstraint activateConstraints:@[
                    [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                    [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                    [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                    [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                ]];
                // 创建桥接器
                ADWebViewBridge *bridge = [[ADWebViewBridge alloc] initWithWebView:webView
                                                                         bridgeName:@"ADNative"
                                                                      messageHandler:messageHandler];
                objc_setAssociatedObject(webView, ADWebViewBridgeKey, bridge, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
                // 原生方案: 直接用 NSData 读取文件
                NSString *path = [NSString stringWithUTF8String:htmlPath];
                NSString *basePath = [path stringByDeletingLastPathComponent];
                NSURL *baseURL = [NSURL fileURLWithPath:basePath];
                NSError *error = nil;
                NSData *data = [NSData dataWithContentsOfFile:path options:0 error:&error];
                if (data && !error) {
                    NSString *htmlString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                    if (htmlString) {
                        [webView loadHTMLString:htmlString baseURL:baseURL];
                    } else {
                        // 如果不是 UTF-8, 尝试其他编码
                        htmlString = [[NSString alloc] initWithData:data encoding:NSISOLatin1StringEncoding];
                        if (htmlString) {
                            [webView loadHTMLString:htmlString baseURL:baseURL];
                        } else {
                            [webView loadHTMLString:@"<h1>Encoding Error</h1><p>Unsupported file encoding</p>" baseURL:nil];
                        }
                    }
                } else {
                    // 读取失败, 显示错误信息
                    NSString *errorMsg = [NSString stringWithFormat:@"<h1>File Not Found</h1><p>Path: %s<br>Error: %@</p>", 
                                          htmlPath, error.localizedDescription ?: @"Unknown error"];
                    [webView loadHTMLString:errorMsg baseURL:nil];
                }

                return bridge;
            }
*/
/*
            // 简化版
            inline id loadhtml(UIView* container, const char* htmlPath) {
                return loadhtml(container, htmlPath, nil);
            }
*/
/*
            // 重載版
            inline id loadhtml(UIView* container, const char* htmlPath, bool analink = false) {
                std::string content = AD::fs::read_all(htmlPath, 1);
                if (analink) {
                    return loadtext(container, content.c_str());
                } else {
                    return loadhtml(container, htmlPath, nil);
                }
            }
*/
            inline id loadhtml(UIView* container, const char* htmlPath,
                               void (^messageHandler)(NSString *name, id body, void (^callback)(NSString *, id, NSString *))) {
                UIWebView *webView = [[UIWebView alloc] initWithFrame:CGRectZero];
                webView.scalesPageToFit = YES;
                webView.translatesAutoresizingMaskIntoConstraints = NO;
                [container addSubview:webView];
                [NSLayoutConstraint activateConstraints:@[
                    [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                    [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                    [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                    [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                ]];

                __block ADWebViewBridge *bridge;
                bridge = [[ADWebViewBridge alloc] initWithWebView:webView
                                                       bridgeName:@"ADNative"
                                                       messageHandler:^(NSString *name, id body) {
                        if ([body isKindOfClass:[NSDictionary class]] && messageHandler) {
                            NSString *callbackId = body[@"callbackId"];
                            NSDictionary *params = body[@"params"];
                            NSString *method = body[@"method"];
                            // 处理文件选择
                            if ([method isEqualToString:@"chooseFile"]) {
                                [bridge showDocumentPickerWithCallbackId:callbackId];
                            } else if ([method isEqualToString:@"chooseImage"]) {
                                // 显示选择菜单
                                UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"选择图片"
                                                            message:nil
                                                            preferredStyle:UIAlertControllerStyleActionSheet];
                            if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
                                [alert addAction:[UIAlertAction actionWithTitle:@"拍照" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
                                    [bridge showImagePickerWithSourceType:UIImagePickerControllerSourceTypeCamera callbackId:callbackId];
                                }]];
                            }
                            [alert addAction:[UIAlertAction actionWithTitle:@"从相册选择" style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
                                [bridge showImagePickerWithSourceType:UIImagePickerControllerSourceTypePhotoLibrary callbackId:callbackId];
                            }]];
                            [alert addAction:[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil]];
                            UIViewController *topVC = [bridge getTopViewController];
                            [topVC presentViewController:alert animated:YES completion:nil];
                        } else if ([method isEqualToString:@"readFile"]) {
                            NSString *filePath = params[@"path"];
                            [bridge readFileContent:filePath callbackId:callbackId];
                        } else {
                            messageHandler(name, body, ^(NSString *cbId, id result, NSString *error) {
                                [bridge sendCallbackToJS:cbId ?: callbackId result:result error:error];
                            });
                        }
                    } else if (messageHandler) {
                        messageHandler(name, body, nil);
                    }
                }];

                objc_setAssociatedObject(webView, ADWebViewBridgeKey, bridge, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
                NSString *path = [NSString stringWithUTF8String:htmlPath];
                NSString *basePath = [path stringByDeletingLastPathComponent];
                NSURL *baseURL = [NSURL fileURLWithPath:basePath];
                NSData *data = [NSData dataWithContentsOfFile:path];
                if (data) {
                    NSString *htmlString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
                    if (htmlString) {
                        [webView loadHTMLString:htmlString baseURL:baseURL];
                    } else {
                        [webView loadHTMLString:@"<h1>Encoding Error</h1>" baseURL:nil];
                    }
                } else {
                    [webView loadHTMLString:@"<h1>File Not Found</h1>" baseURL:nil];
                }
                return bridge;
            }
            // 保留旧版本兼容
            inline id loadhtml(UIView* container, const char* htmlPath) {
                return loadhtml(container, htmlPath, nil);
            }
          # endif // ad_objc_use_web
          # ifdef ad_objc_use_scheme
            namespace url_scheme {
                inline WKWebView* create(UIView* container, id navigationDelegate, const char* scheme_name, const char* root_dir) {
                    WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
                    if (@available(iOS 14.0, *)) {
                        WKWebpagePreferences *preferences = [[WKWebpagePreferences alloc] init];
                        preferences.allowsContentJavaScript = YES; 
                        config.defaultWebpagePreferences = preferences;
                    } else {
                        config.preferences.javaScriptEnabled = YES;
                    }

                    NSString *basePathStr = [NSString stringWithUTF8String:root_dir];
                    NSString *schemeStr = [NSString stringWithUTF8String:scheme_name];
                    ADLocalSchemeHandler *handler = [[ADLocalSchemeHandler alloc] initWithBasePath:basePathStr];
                    [config setURLSchemeHandler:handler forURLScheme:schemeStr];
                    WKWebView *webView = [[WKWebView alloc] initWithFrame:CGRectZero configuration:config];
                    webView.navigationDelegate = (id<WKNavigationDelegate>)navigationDelegate;
                    webView.translatesAutoresizingMaskIntoConstraints = NO;
                    [container addSubview:webView];
                    [NSLayoutConstraint activateConstraints:@[
                        [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                        [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                        [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                        [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                    ]];
                    return webView;
                }

                inline void call(WKWebView* webView, const char* scheme_name, const char* entry_file) {
                    NSString *urlStr = [NSString stringWithFormat:@"%s://%s", scheme_name, entry_file];
                    NSURL *url = [NSURL URLWithString:urlStr];
                    [webView loadRequest:[NSURLRequest requestWithURL:url]];
                }
            } // namespace url_scheme
            namespace scheme = url_scheme;
          # endif // ad_objc_use_scheme
        } // namespace web
        namespace make {
            UIButton * _Nullable buttonimg(CGFloat x, CGFloat y, CGFloat width, CGFloat height, 
                                           NSString * _Nonnull normalImageName, 
                                           NSString * _Nullable highlightedImageName) {
               if (!normalImageName || normalImageName.length == 0) {
                   return nil;
               }

               UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
               button.frame = CGRectMake(x, y, width, height);

              UIImage *normalImage = [UIImage imageNamed:normalImageName];
              if (normalImage) {
                  [button setImage:normalImage forState:UIControlStateNormal];
              }
              if (highlightedImageName && highlightedImageName.length > 0) {
                  UIImage *highlightedImage = [UIImage imageNamed:highlightedImageName];
                  if (highlightedImage) {
                      [button setImage:highlightedImage forState:UIControlStateHighlighted];
                  } else {
                      [button setImage:normalImage forState:UIControlStateHighlighted];
                  }
              } else {
                  [button setImage:normalImage forState:UIControlStateHighlighted];
              }

              return button;
          }

          namespace set {
              UIImageView * _Nullable mainui_back(NSString * _Nonnull imageName, CGRect frame) {
                  if (!imageName || imageName.length == 0) {
                      return nil;
                  }
                  
                  UIImageView *imageView = [[UIImageView alloc] initWithFrame:frame];
                  imageView.image = [UIImage imageNamed:imageName];
                  imageView.contentMode = UIViewContentModeScaleAspectFill;
                  imageView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
                  
                  return imageView;
              }

              constexpr auto& mainui_bg = mainui_back;
              constexpr auto& mainbg = mainui_back;
              constexpr auto& mainuibg = mainui_back;
          } // namespace set
        } // namespace make
        namespace set = make::set;
        namespace theme {
            void black_white(bool forceDarkMode = false) {
                bool isDarkMode = forceDarkMode;
                if (!forceDarkMode) {
                    if (@available(iOS 13.0, *)) {
                        UITraitCollection *trait = [UITraitCollection currentTraitCollection];
                        isDarkMode = (trait.userInterfaceStyle == UIUserInterfaceStyleDark);
                    }
                }
                glClearColor(isDarkMode ? 0.0f : 1.0f, 
                             isDarkMode ? 0.0f : 1.0f, 
                             isDarkMode ? 0.0f : 1.0f, 
                             1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            constexpr auto& appearance = black_white;

            inline void darkmode() { black_white(true); }
            inline void lightmode() { black_white(false); }
            inline void automode() { black_white(); }  // 跟隨系統
        } // namespace theme
        namespace Theme = theme;
    } // namespace ui
    namespace web = ui::web;
  # else
    # error "only apple platform and use objc/objc++ code, only then can it be used"
  # endif // __OBJC__ && __APPLE__ && ad_use_objc_code || ad_use_objc
} // namespace AD

# pragma GCC diagnostic pop

# endif // _ADLIBCXX_OBJC_TESTAPI_HPP_

// ADlibc++-objc-testapi.hpp

# ifndef _ADLIBCXX_OBJC_TESTAPI_HPP_
# define _ADLIBCXX_OBJC_TESTAPI_HPP_

# if defined(__OBJC__) && defined(__APPLE__)
# import <Foundation/Foundation.h>
# import <WebKit/WebKit.h>
# import <UIKit/UIKit.h>
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

namespace AD {
  # if defined(__OBJC__) && defined(__APPLE__) && defined(ad_use_objc_code) || defined(ad_use_objc)
    namespace ui {
        namespace web {
            // 載入 HTML 文件
            void loadhtml(UIView* container, id navigationDelegate, const char* htmlPath) {
                WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
                if (@available(iOS 14.0, *)) {
                    WKWebpagePreferences *preferences = [[WKWebpagePreferences alloc] init];
                    preferences.allowsContentJavaScript = YES; 
                    config.defaultWebpagePreferences = preferences;
                } else {
                    config.preferences.javaScriptEnabled = YES;
                }

                [config.preferences setValue:@YES forKey:@"allowFileAccessFromFileURLs"];
                WKWebView *webView = [[WKWebView alloc] initWithFrame:CGRectZero 
                                                configuration:config];
                webView.navigationDelegate = (id<WKNavigationDelegate>)navigationDelegate;
                webView.translatesAutoresizingMaskIntoConstraints = NO;
                [container addSubview:webView];
                // 添加約束, 填滿整個容器
                [NSLayoutConstraint activateConstraints:@[
                    [webView.topAnchor constraintEqualToAnchor:container.topAnchor],
                    [webView.bottomAnchor constraintEqualToAnchor:container.bottomAnchor],
                    [webView.leadingAnchor constraintEqualToAnchor:container.leadingAnchor],
                    [webView.trailingAnchor constraintEqualToAnchor:container.trailingAnchor]
                ]];

                NSString *path = [NSString stringWithUTF8String:htmlPath];
                NSURL *htmlURL = [NSURL fileURLWithPath:path];
                // 檢查文件是否存在
                if ([[NSFileManager defaultManager] fileExistsAtPath:path]) {
                    [webView loadFileURL:htmlURL 
                        allowingReadAccessToURL:[htmlURL URLByDeletingLastPathComponent]];
                } else {
                    NSLog(@"HTML file not found: %s", htmlPath);
                    // 顯示錯誤信息
                    [webView loadHTMLString:@"<h1>File Not Found</h1><p>HTML file does not exist.</p>" 
                                    baseURL:nil];
                }
            }
            // 載入 HTML 字串
            void loadtext(UIView* container, const char* htmlString) {
                WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
                WKWebView *webView = [[WKWebView alloc] initWithFrame:container.bounds 
                                                        configuration:config];
                [container addSubview:webView];
                NSString *html = [NSString stringWithUTF8String:htmlString];
                [webView loadHTMLString:html baseURL:nil];
            }
            // 載入 HTML 字串(帶 baseURL)
            void loadtext(UIView* container, const char* htmlString, const char* baseURL) {
                WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
                WKWebView *webView = [[WKWebView alloc] initWithFrame:container.bounds 
                                                        configuration:config];
                [container addSubview:webView];
                NSString *html = [NSString stringWithUTF8String:htmlString];
                NSURL *url = baseURL ? [NSURL URLWithString:[NSString stringWithUTF8String:baseURL]] : nil;
                [webView loadHTMLString:html baseURL:url];
            }
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
    } // namespace ui
    namespace web = ui::web;
  # else
    # error "only apple platform and use objc/objc++ code, only then can it be used"
  # endif // __OBJC__ && __APPLE__ && ad_use_objc_code || ad_use_objc
} // namespace AD

# endif // _ADLIBCXX_OBJC_TESTAPI_HPP_

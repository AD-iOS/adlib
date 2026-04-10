/*
c++ -arch arm64 -fobjc-arc \
    -framework UIKit -framework WebKit -framework JavaScriptCore -framework CoreGraphics \
    -lroothide -lboost_json \
    -lssl -lcrypto -lz \
    -lnghttp2 -lssh2 -lrtmp -lzstd \
    -lbrotlidec -lbrotlicommon \
    -lcares -lidn2 -lunistring \
    -lcurl \
    -Wl,-rpath,/usr/lib \
    -Wl,-rpath,/lib \
    -Wl,-rpath,/usr/local/lib \
    -Wl,-rpath,/var/jb/usr/lib \
    -Wl,-rpath,/var/jb/lib \
    -Wl,-rpath,/var/jb/usr/local/lib \
    -Wl,-rpath,/var/jb/opt/local/lib \
    -Wl,-rpath,/opt/local/lib \
    test2.mm -o test \
    && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha1 -Hsha256 test && chmod 6755 test
or: -old
c++ -arch arm64 -fobjc-arc \
    -framework UIKit -framework WebKit -framework JavaScriptCore -framework CoreGraphics \
    -lroothide -lboost_json \
    -lssl -lcrypto -lz \
    -lnghttp2 -lssh2 -lrtmp -lzstd \
    -lbrotlidec -lbrotlicommon \
    -lcares -lidn2 -lunistring \
    -lcurl \
    -Wl,-undefined,dynamic_lookup \
    test2.mm -o test \
    && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha1 -Hsha256 test && chmod 6755 test
*/
/*
 * old
rootless: 
c++ -arch arm64 -fobjc-arc -D__rootless__ -framework UIKit -framework CoreGraphics -framework WebKit -framework JavaScriptCore -lroothide test2.mm -o test \
    && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha256 -Hsha1 -M test && chmod 6755 test
roothide: 
c++ -arch arm64 -fobjc-arc -D__roothide__ -framework UIKit -framework CoreGraphics -framework WebKit -framework JavaScriptCore -lroothide test2.mm -o test \
    && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha256 -Hsha1 -M test && chmod 6755 test
*/

# import "test2.h"

# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# import <WebKit/WebKit.h>
# import <SafariServices/SafariServices.h>
# define ad_comuse
# define ad_use_objc
# define load_ad_system
# define ad_user
# import <AD/ad.h>
// # import "ADlibc++-testapi.hpp" // 測試完成, 已經合併到 adlibcxx-makeupforit.hpp 中, 由 ADlibc++.hpp 自動包含
# import <AD/ADlibc++-objc.hpp>
# define ad_objc_use_web
# import "ADlibc++-objc-testapi.hpp"
# import "ADlibc++-shell-testapi.hpp"
# import <unistd.h>
# import <string>
# import <roothide/roothide.h>
# import <cstdlib>

namespace ad = AD;

std::string get_jbarch() {
    return ad::system("[ -f '/etc/profile' ] && source /etc/profile || { [ -f '/var/jb/etc/profile' ] && source /var/jb/etc/profile || echo 'Where the fuck `profile`?' 1>&2; } && dpkg --print-architecture | tr -d '\n' || echo 'unknown'", 1);
}

std::string getapphome() {
    # if defined(__roothide__)
        return jbroot("/opt/testapp");
    # elif defined(__rootless__)
        return "/var/jb/opt/testapp";
    # else
        std::string arch = get_jbarch();
        if (arch == "iphoneos-arm64") { // rootless
            return "/var/jb/opt/testapp";
        } else if (arch == "iphoneos-arm64e") { // roothide
            return jbroot("/opt/testapp");
        } else { // rootful
            return "/opt/testapp";
        }
    # endif
}

std::string apphome = getapphome();

std::string apphome_doc = apphome + "/Documents";
std::string apphome_lib = apphome + "/Library";
std::string apphome_sysdata = apphome + "/SystemData";
std::string apphome_tmp = apphome + "/tmp";

@interface testViewController : UIViewController </* WKNavigationDelegate, */UIWebViewDelegate>
@end

@implementation testViewController

- (void)viewDidLoad {
/*
UIButton *btn1 = buttonimg(100, 100, 100, 100, @"icon_normal", nil);

UIButton *btn2 = buttonimg(100, 100, 100, 100, @"icon_normal", @"icon_highlighted");

UIButton *btn3 = buttonimg(100, 100, 100, 100, @"icon_normal", @"");
[self.view addSubview:button];
*/

    [super viewDidLoad];

    [self.view addSubview:bgView];
    [self.view sendSubviewToBack:bgView];

    self.view.backgroundColor = AD::ui::color::white();
    self.title = @"Test2";

    UILabel* label1 = AD::ui::make::label(@"Hello user, this is the first text\n");
    label1.textColor = AD::ui::color::black();

    UILabel* label2 = [self test2];

    UIButton* button1 = AD::ui::make::button(@"點擊我顯示彈窗!");
    [button1 setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button1 addTarget:self action:@selector(showMyAlert) forControlEvents:UIControlEventTouchUpInside];

    UIButton* button2 = AD::ui::make::button(@"測試彈窗");
    [button2 setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button2 addTarget:self action:@selector(test_show_myalert) forControlEvents:UIControlEventTouchUpInside];

    UIButton* button3 = ad::ui::make::button(@"test ai");
    [button3 setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button3 addTarget:self action:@selector(testweb_html) forControlEvents:UIControlEventTouchUpInside];

    UILabel* label3 = [self show_myself_path];

    UIButton* button4 = ad::ui::mk::button(@"become root");
    [button4 setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button4 addTarget:self action:@selector(become_root) forControlEvents:UIControlEventTouchUpInside];

    UIButton* button5 = ad::ui::mk::button(@"jb arch");
    [button5 setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button5 addTarget:self action:@selector(show_jbarch) forControlEvents:UIControlEventTouchUpInside];

    [self.view addSubview:label1];
    [self.view addSubview:label2];
    [self.view addSubview:button1];
    [self.view addSubview:button2];
    [self.view addSubview:button3];
    [self.view addSubview:label3];
    [self.view addSubview:button4];
    [self.view addSubview:button5];

    [NSLayoutConstraint activateConstraints:@[
        [label1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label1.centerYAnchor constraintEqualToAnchor:self.view.centerYAnchor constant:-80],
        [label2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label2.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:20],
        [button1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button1.topAnchor constraintEqualToAnchor:label2.bottomAnchor constant:60],
        [button2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button2.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:40],
        [button3.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button3.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:80],
        [label3.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label3.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:80],
        [button4.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button4.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:120],
        [button5.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button5.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:140]
    ]];
}

- (UILabel *)test2 {
    UILabel* label = AD::ui::make::label(@"Hello user, this is test2 function\n");
    label.textColor = AD::ui::color::black();
    return label;
}

- (void)showMyAlert {
    AD::ui::alert::show(self, @"溫馨提示", @"這是一個彈窗內容!你成功點擊了按鈕");
}

- (UILabel *) show_myself_path {
    std::string pathStr = ad::shcmd::pwd(true);
    NSString* path = @(pathStr.c_str());

    UILabel* label = AD::ui::make::label(@"path: ", path);
    label.textColor = AD::ui::color::black();
    return label;
}

- (void) test_show_myalert {
    // because there is no data directory, so use `opt` as data dir
    AD::user::change("root", "wheel");

    std::string test_sh_path = apphome_doc + "/test.sh";
    ad::fs::touch(test_sh_path);

    std::string src_test_sh_code = "#!/bin/sh\n"
        "echo \"hello, this is one test file\" >> result.txt\n";
    ad::fs::append(test_sh_path, src_test_sh_code);

    chmod(test_sh_path.c_str(), 0755);

    bool sh_exists = ad::aux::is_file("/var/jb/usr/bin/sh") || 
                    (ad::aux::is_file(jbroot("/usr/bin/sh")) || 
                    (ad::aux::is_file("/usr/bin/sh")) == 1);

    if (sh_exists) {
        ad::command::dash(test_sh_path.c_str());
        NSString* message = [NSString stringWithFormat:@"run success, please check result.txt at %s", apphome_doc.c_str()];
        AD::ui::alert::show(self, @"info", message);
    } else {
        ad::cerr << "run failure or file not exist" << ad::endl;
        AD::ui::alert::show(self, @"error", @"run failure or file not exist");
    }
}

- (void) show_jbarch {
    std::string arch_output = get_jbarch();
    // NSString* jbarch = @(arch_output.c_str());
    AD::ui::alert::show(self, @"jbarch", @(arch_output.c_str()));
}


- (void)testweb_html {
    UIViewController *webVC = [[UIViewController alloc] init];
    webVC.title = @"test ai";
    
    std::string www = apphome_doc + "/www";
    std::string htmfile = www + "/index.html";
    
    ad::ui::web::loadhtml(webVC.view, htmfile.c_str(), ^(NSString *name, id body, void (^callback)(NSString *, id, NSString *)) {
        if (![body isKindOfClass:[NSDictionary class]]) return;
        
        NSString *method = body[@"method"];
        NSDictionary *params = body[@"params"];
        NSString *callbackId = body[@"callbackId"];
        
        // 获取 www 下的 data 目录
        NSString *wwwPath = [NSString stringWithUTF8String:www.c_str()];
        NSString *dataPath = [wwwPath stringByAppendingPathComponent:@"data"];
        NSString *apiFile = [dataPath stringByAppendingPathComponent:@"api.txt"];
        NSString *historyDir = [dataPath stringByAppendingPathComponent:@"history"];
        
        if ([method isEqualToString:@"saveKey"]) {
            NSString *key = params[@"key"];
            NSError *err = nil;
            [key writeToFile:apiFile atomically:YES encoding:NSUTF8StringEncoding error:&err];
            callback(callbackId, @{@"success": @(err == nil)}, err ? err.localizedDescription : nil);
        }
        else if ([method isEqualToString:@"getKey"]) {
            NSString *key = [NSString stringWithContentsOfFile:apiFile encoding:NSUTF8StringEncoding error:nil];
            callback(callbackId, key ?: @"", nil);
        }
        else if ([method isEqualToString:@"clearKey"]) {
            [[NSFileManager defaultManager] removeItemAtPath:apiFile error:nil];
            callback(callbackId, @{@"success": @YES}, nil);
        }
        else if ([method isEqualToString:@"saveHistory"]) {
            NSString *sessionId = params[@"sessionId"];
            id data = params[@"data"];
            [[NSFileManager defaultManager] createDirectoryAtPath:historyDir withIntermediateDirectories:YES attributes:nil error:nil];
            NSString *file = [historyDir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.json", sessionId]];
            NSLog(@"[SAVE PATH] %@", file);  // 打印路径
            NSData *json = [NSJSONSerialization dataWithJSONObject:data options:NSJSONWritingPrettyPrinted error:nil];
            BOOL ok = [json writeToFile:file atomically:YES];
            callback(callbackId, @{@"success": @(ok)}, ok ? nil : @"write failed");
        }
        else if ([method isEqualToString:@"loadHistory"]) {
            NSString *sessionId = params[@"sessionId"];
            NSString *file = [historyDir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.json", sessionId]];
            NSLog(@"[LOAD PATH] %@", file);  // 打印路径
            NSData *json = [NSData dataWithContentsOfFile:file];
            id result = json ? [NSJSONSerialization JSONObjectWithData:json options:0 error:nil] : nil;
            callback(callbackId, result ?: @{}, result ? nil : @"not found");
        }
        else if ([method isEqualToString:@"listHistories"]) {
            [[NSFileManager defaultManager] createDirectoryAtPath:historyDir withIntermediateDirectories:YES attributes:nil error:nil];
            NSArray *files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:historyDir error:nil];
            NSMutableArray *list = [NSMutableArray array];
            for (NSString *file in files) {
                if ([file hasSuffix:@".json"]) {
                    NSString *filePath = [historyDir stringByAppendingPathComponent:file];
                    NSDictionary *attrs = [[NSFileManager defaultManager] attributesOfItemAtPath:filePath error:nil];
                    NSString *sessionId = [file stringByDeletingPathExtension];
                    [list addObject:@{
                        @"id": sessionId,
                        @"timestamp": attrs[NSFileModificationDate] ? [attrs[NSFileModificationDate] description] : @""
                    }];
                }
            }
            callback(callbackId, list, nil);
        }
        else if ([method isEqualToString:@"deleteHistory"]) {
            NSString *sessionId = params[@"sessionId"];
            NSString *file = [historyDir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.json", sessionId]];
            BOOL ok = [[NSFileManager defaultManager] removeItemAtPath:file error:nil];
            callback(callbackId, @{@"success": @(ok)}, ok ? nil : @"delete failed");
        }
        else {
            callback(callbackId, nil, @"unknown method");
        }
    });
    
    [self.navigationController pushViewController:webVC animated:YES];
}

- (void)dismissFullscreen {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void) become_root {
    ad::user::change("root", "wheel");
    AD::ui::alert::show(self, @"become root", @"success");
}

/*

#pragma mark - UIWebViewDelegate

- (void)webViewDidStartLoad:(UIWebView *)webView {
    NSLog(@"UIWebView 开始加载");
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    NSLog(@"UIWebView 加载完成!");
    
    // 获取页面标题
    NSString *title = [webView stringByEvaluatingJavaScriptFromString:@"document.title"];
    NSLog(@"页面标题: %@", title);
    
    // 获取页面内容
    NSString *body = [webView stringByEvaluatingJavaScriptFromString:@"document.body.innerHTML"];
    NSLog(@"页面内容: %@", body);
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    NSLog(@"UIWebView 加载失败: %@", error);
    NSLog(@"错误描述: %@", error.localizedDescription);
}
*/

@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.window = AD::ui::make::window();

    testViewController* vc = [[testViewController alloc] init];
    self.window.rootViewController = AD::ui::make::navigationController(vc);

    [self.window makeKeyAndVisible];
    return YES;
}
@end

int main(int argc, char* argv[]) {
    ad::www::init();
    ad::fs::mkdir(apphome);
    ad::fs::mkdir(apphome_doc);
    ad::fs::mkdir(apphome_lib);
    ad::fs::mkdir(apphome_sysdata);
    ad::fs::mkdir(apphome_tmp);

    return AD::ui::run(argc, argv, [AppDelegate class]);
}
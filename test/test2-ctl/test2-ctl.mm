/*
c++ -fobjc-arc -framework UIKit -framework CoreGraphics -framework Foundation -framework WebKit -lroothide test2-ctl.mm -o test2-ctl && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha256 -Hsha1 -M test2-ctl && chmod 6755 test2-ctl
*/

# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# import <WebKit/WebKit.h>
# define ad_comuse
# define ad_use_objc
# define load_ad_system
# define ad_user
# define ad_cli
# import <AD/ad.h>
# import "ADlibc++-testapi.hpp"
# import <AD/ADlibc++-objc.hpp>
# import "ADlibc++-objc-testapi.hpp"
# import "ADlibc++-shell-testapi.hpp"
# import <unistd.h>
# import <string>
# import <roothide/roothide.h>

@interface testViewController : UIViewController <WKNavigationDelegate>
@end

// 修改 testViewController
@implementation testViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 檢查命令列參數
    AD::cli::init(UIApplication.sharedApplication.argc, 
                  UIApplication.sharedApplication.argv);
    
    if (ad::cli::arg == "show-window") {
        [self test2_show_window];
    }
}

- (void)test2_show_window {
    ad::ui::alert::show(self, @"這是標題", @"這是彈窗的詳細內容");
}

@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window.rootViewController = [[UIViewController alloc] init];
    [self.window makeKeyAndVisible];
    return YES;
}
@end

int main(int argc, char* argv[]) {
    AD::cli::init(argc, argv);
    if (ad::cli::arg == "show-window") {
        @autoreleasepool {
            dispatch_async(dispatch_get_main_queue(), ^{
                UIWindow *window = AD::ui::make::window();
                UIViewController *vc = [[UIViewController alloc] init];
                window.rootViewController = vc;
                window.windowLevel = UIWindowLevelAlert;
                [window makeKeyAndVisible];
                
                AD::ui::alert::show(vc, @"這是標題", @"這是彈窗的詳細內容");
            });
            
            [[NSRunLoop mainRunLoop] run];
        }
        return 0;
    }

    if (ad::cli::arg == "test-alert") {
        @autoreleasepool {
            dispatch_async(dispatch_get_main_queue(), ^{
                UIWindow *window = AD::ui::make::window();
                UIViewController *vc = [[UIViewController alloc] init];
                window.rootViewController = vc;
                window.windowLevel = UIWindowLevelAlert;
                [window makeKeyAndVisible];
                
                AD::ui::alert::confirm(vc, @"確認", @"確定要執行嗎？", ^(BOOL confirmed) {
                    NSLog(@"使用者%@了", confirmed ? @"確定" : @"取消");
                    exit(0);
                });
            });
            
            [[NSRunLoop mainRunLoop] run];
        }
        return 0;
    }

    return AD::ui::run(argc, argv, [AppDelegate class]);
}
/*
c++ -fobjc-arc -framework UIKit -framework CoreGraphics -framework Foundation -lroothide main.mm -o a1 && ldid -S$jb/opt/ad/share/ens/ens-root.xml -Hsha256 -Hsha1 -M a1 && chmod 6755 a1
*/

# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# define ad_comuse
# define ad_use_objc
# define load_ad_system
# define ad_user
# import <AD/ad.h>
# import <AD/ADlibc++-objc.hpp>
# import "ADlibc++-shell-testapi.hpp"
# import "_find.hpp"
# import <unistd.h>
# import <string>
# import <roothide/roothide.h>

namespace ad = AD;

const char* get_jb_path() {
# if defined(__rootless__) || defined(__ios_rl__)
    return "/var/jb";
# elif defined(__roothide__) || defined(__ios_rh__)
    return jbroot("/var/jb");
# else
    static std::string path;
    if (path.empty()) {
        FILE* fp = popen("dpkg --print-architecture 2>/dev/null", "r");
        if (fp) {
            char arch[64] = {0};
            if (fgets(arch, sizeof(arch), fp)) {
                std::string arch_str = arch;
                arch_str.erase(arch_str.find_last_not_of("\n\r") + 1);
                if (arch_str == "iphoneos-arm64e") {
                    path = jbroot("/var/jb");
                } else if (arch_str == "iphoneos-arm64") {
                    path = "/var/jb";
                } else {
                    path = "";
                }
            }
            pclose(fp);
        }
    }
    return path.c_str();
# endif
}

# define jb get_jb_path()
std::string jba1 = std::string(get_jb_path()) + "/a1";
std::string jba1_config = jba1 + "/config.conf";

@interface a1ViewController : UIViewController
@end

@implementation a1ViewController

- (void) viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = AD::ui::color::white();
    self.title = @"A1";

    UILabel* label1 = ad::ui::mk::label(@"this is A1 CTL APP Version\n");
    label1.textColor = ad::ui::color::black();

    UILabel* label2 = [self test2];

    [self.view addSubview:label1];
    [self.view addSubview:label2];
/*
    [self.view addSubview:button1];
    [self.view addSubview:button2];
    [self.view addSubview:button3];
    [self.view addSubview:label3];
*/
    [NSLayoutConstraint activateConstraints:@[
        [label1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label1.centerYAnchor constraintEqualToAnchor:self.view.centerYAnchor constant:-80],
        [label2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label2.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:20]
/*
        [button1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button1.topAnchor constraintEqualToAnchor:label2.bottomAnchor constant:20],
        [button2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button2.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:20],
        [button3.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button3.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:60],
        [label3.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label3.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:60],
*/
    ]];
}

- (UILabel *)test2 {
/*
    char* char_newjba1 = realpath(jba1.c_str(), NULL);
    if (!char_newjba1) {
        UILabel *label = AD::ui::make::label(@"循環模式等待時間: 5 (路徑錯誤)");
        label.textColor = AD::ui::color::black();
        return label;
    }

    std::string jba1_config = std::string(char_newjba1) + "/config.conf";
    free(char_newjba1);
    
    std::string loop_sleep_int = ad::read_get_env_val(jba1_config, "Loop_Sleep_Interval", '=');
    
    if (loop_sleep_int.empty()) {
        loop_sleep_int = "5";
    }
*/
    if (ad::read_get_env_val(jba1_config, "export Loop_Sleep_Interval", '=') == "") {
        std::string loop_sleep_int = "get failure";
        UILabel *label = AD::ui::make::label([NSString stringWithFormat:@"循環模式等待時間: %s", loop_sleep_int.c_str()]);
        label.textColor = AD::ui::color::black();
        return label;
    } else {
        std::string loop_sleep_int = ad::read_get_env_val(jba1_config, "export Loop_Sleep_Interval", '=');
        UILabel *label = AD::ui::make::label([NSString stringWithFormat:@"循環模式等待時間: %s", loop_sleep_int.c_str()]);
        label.textColor = AD::ui::color::black();
        return label;
    }
}

- (UILabel *) show_myself_path {
    std::string pathStr = ad::shcmd::pwd(true);
    NSString* path = @(pathStr.c_str());

    UILabel* label = AD::ui::make::label(@"path: ", path);
    label.textColor = AD::ui::color::black();
    return label;
}

- (void) test_show_myalert {
    // AD::user::change("root", "wheel");
    AD::ui::alert::show(self, @"error", @"run failure or file not exist");
}

@end

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window.rootViewController = [[a1ViewController alloc] init];
    [self.window makeKeyAndVisible];
    return YES;
}
@end

int main(int argc, char* argv[]) {
    AD::user::change("root", "wheel");
    return AD::ui::run(argc, argv, [AppDelegate class]);
}
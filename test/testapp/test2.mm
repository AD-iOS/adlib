/*
c++ -fobjc-arc -framework UIKit -framework CoreGraphics -framework Foundation -lroothide test2.mm -o test && ldid -S/opt/ad/share/ens/ens-root.xml -Hsha256 -Hsha1 -M test && chmod 6755 test
*/

# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# define ad_comuse
# define ad_use_objc
# define ad_system
# define ad_user
# import <AD/ad.h>
# import <AD/ADlibc++-objc.hpp>
# import <unistd.h>
# import <string>
# import <roothide/roothide.h>

@interface testViewController : UIViewController
@end

@implementation testViewController

- (void)viewDidLoad {
    [super viewDidLoad];
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

    [self.view addSubview:label1];
    [self.view addSubview:label2];
    [self.view addSubview:button1];
    [self.view addSubview:button2];

    [NSLayoutConstraint activateConstraints:@[
        [label1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label1.centerYAnchor constraintEqualToAnchor:self.view.centerYAnchor constant:-80],
        [label2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label2.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:20],
        [button1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button1.topAnchor constraintEqualToAnchor:label2.bottomAnchor constant:20],
        [button2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button2.topAnchor constraintEqualToAnchor:button1.bottomAnchor constant:20]
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

- (void) test_show_myalert {
    // because there is no data directory, so use `opt` as data dir
    AD::user::change("root", "wheel");

    std::string apphome;
    #if defined(__roothide__)
        apphome = jbroot("/opt/testapp");
    #elif defined(__rootless__)
        apphome = "/var/jb/opt/testapp";
    #else
        FILE* fp = popen("dpkg --print-architecture 2>/dev/null", "r");
        if (fp) {
            char arch[64] = {0};
            if (fgets(arch, sizeof(arch), fp) != nullptr) {
                std::string arch_str = arch;
                arch_str.erase(arch_str.find_last_not_of("\n\r") + 1);
                if (arch_str == "iphoneos-arm64e") {
                    apphome = jbroot("/opt/testapp");
                } else if (arch_str == "iphoneos-arm64") {
                    apphome = "/var/jb/opt/testapp";
                } else {
                    apphome = "/opt/testapp";
                }
            } else {
                apphome = "/opt/testapp";
            }
            pclose(fp);
        } else {
            apphome = "/opt/testapp";
        }
    #endif

    std::string apphome_doc = apphome + "/Documents";
    std::string apphome_lib = apphome + "/Library";
    std::string apphome_sysdata = apphome + "/SystemData";
    std::string apphome_tmp = apphome + "/tmp";

    ad::fs::mkdir(apphome);
    ad::fs::mkdir(apphome_doc);
    ad::fs::mkdir(apphome_lib);
    ad::fs::mkdir(apphome_sysdata);
    ad::fs::mkdir(apphome_tmp);

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
    return AD::ui::run(argc, argv, [AppDelegate class]);
}
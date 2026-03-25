/*
c++ -fobjc-arc -framework UIKit -framework CoreGraphics -framework Foundation -lroothide test2.mm -o test && ldid -S/opt/ad/share/ens/ens.xml -Hsha256 -Hsha1 -M test
*/

# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# define ad_comuse
# define ad_use_objc
# import <AD/ad.h>
# import <AD/ADlibc++-objc.hpp>

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

    UIButton* button = AD::ui::make::button(@"點擊我顯示彈窗!");
    [button setTitleColor:AD::ui::color::sysblue() forState:UIControlStateNormal];
    [button addTarget:self action:@selector(showMyAlert) forControlEvents:UIControlEventTouchUpInside];

    [self.view addSubview:label1];
    [self.view addSubview:label2];
    [self.view addSubview:button];

    [NSLayoutConstraint activateConstraints:@[
        [label1.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label1.centerYAnchor constraintEqualToAnchor:self.view.centerYAnchor constant:-80],
        [label2.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [label2.topAnchor constraintEqualToAnchor:label1.bottomAnchor constant:20],
        [button.centerXAnchor constraintEqualToAnchor:self.view.centerXAnchor],
        [button.topAnchor constraintEqualToAnchor:label2.bottomAnchor constant:20]
    ]];
}

- (UILabel *)test2 {
    UILabel* label = AD::ui::make::label(@"Hello user, this is test2 function\n");
    label.textColor = AD::ui::color::black();
    return label;
}

- (void)showMyAlert {
    AD::ui::alert::show(self, @"溫馨提示", @"這是一個彈窗內容！你成功點擊了按鈕。");
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
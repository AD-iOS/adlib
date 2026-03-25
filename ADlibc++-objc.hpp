/*    AD-DEV Public General License
 *       Version 1.0.0, December 2025
 *
 *  Copyright (c) 2025-2026 AD-iOS (ad-ios334@outlook.com) All rights reserved.
 *
 *  (Note: AD, AD-dev, and AD-iOS refer to the same person. Email: ad-ios334@outlook.com)
 *
 *  Hereinafter, the AD-DEV Public General License is referred to as this Agreement or this License. The original source code, executable binaries, and related documentation are collectively referred to as the Software. Use for profit, including sales, leasing, advertising support, etc., is referred to as Commercial Use. Works modified or extended based on the Software are referred to as Derivative Products.
 *
 *  We hereby grant you the following rights, subject to the following conditions, a worldwide, royalty-free, non-exclusive, irrevocable license to:
 *    1. Use, copy, and modify the Software
 *    2. Create derivative works
 *    3. Distribute the Software and derivative works
 *
 *  Conditions:
 *    1. Retain the original author's license and copyright notices
 *    2. The original author assumes no liability
 *    3. Unauthorized commercial use is prohibited
 *    4. All modifications and derivative products based on the Software must use this License and disclose the original source code
 *    5. Disclosure of the original source code is not required when using the Software unmodified (e.g., via APIs, libraries, automatic linkers, etc.)
 *
 *  Anyone may distribute or copy this License file, but may not modify it.
 *  For commercial use, please contact the maintainer of the Software to obtain written and electronic authorization. Note that commercial licensing may involve fees; specific terms shall be negotiated separately.
 *  Violation of this Agreement will automatically terminate the license. Upon termination, all copies of the Software must be destroyed, use of the Software must cease, derivative products must be withdrawn from distribution channels, and distribution of derivative products must stop.
 *
 *  The software is provided "as is", without warranty of any kind, express or implied. In no event shall the authors or copyright holders be liable for any claim, damages, or other liability arising from the use of the software.
 *
 *  If any provision is invalid, it does not affect the validity of the other provisions.
 *  By using, distributing, or modifying the Software, you automatically agree to and intend to comply with this Agreement. If you cannot comply, you must stop using, distributing, or modifying the Software.
 *  This Agreement is governed by and construed in accordance with the laws of the People's Republic of China (without regard to conflict of law principles).
 */
/*
 *
 * ADlibc++-objc.hpp
 * AD‘s C++ private standard library
 * Created by AD on 2026-03-22:19:19
 * Copyright (c) 2025-2026 AD All rights reserved.
 *
 */
/*
 * this is the objc sub-lilary of AD's C++ private standard library
 * It can simplify the application process development of some Apple platforms
 */
/* at present, it is only the first generation */

# ifndef _ADLIBCXX_OBJC_HPP_
# define _ADLIBCXX_OBJC_HPP_
# ifdef __OBJC__
# import <Foundation/Foundation.h>
# import <UIKit/UIKit.h>
# endif // __OBJC__

namespace AD {
  # if defined(ad_use_objc_code) || defined(ad_use_objc)
    namespace ui {
        // MARK: 應用程式入口
        inline int run(int argc, char* argv[], id delegate = nil) {
            @autoreleasepool {
                return UIApplicationMain(argc, argv, nil, 
                    delegate ? NSStringFromClass([delegate class]) : nil);
            }
        }
        // MARK: 視窗建立輔助
        namespace make {
            inline UIWindow* window() {
                return [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
            }

            inline UILabel* label(NSString* text) {
                UILabel* label = [[UILabel alloc] init];
                label.text = text;
                label.numberOfLines = 0;
                label.textAlignment = NSTextAlignmentCenter;
                label.translatesAutoresizingMaskIntoConstraints = NO;

                // 垂直
                [label setContentCompressionResistancePriority:UILayoutPriorityRequired 
                                                       forAxis:UILayoutConstraintAxisVertical];
                [label setContentHuggingPriority:UILayoutPriorityRequired 
                                         forAxis:UILayoutConstraintAxisVertical];

                // 横向
/*
                [label setContentCompressionResistancePriority:UILayoutPriorityRequired 
                                                       forAxis:UILayoutConstraintAxisHorizontal];
                [label setContentHuggingPriority:UILayoutPriorityRequired 
                                         forAxis:UILayoutConstraintAxisHorizontal];
*/
                return label;
            }

            inline UIButton* button(NSString* title) {
                UIButton* button = [UIButton buttonWithType:UIButtonTypeSystem];
                [button setTitle:title forState:UIControlStateNormal];
                button.translatesAutoresizingMaskIntoConstraints = NO;
                return button;
            }

            inline UINavigationController* navigationController(UIViewController* root) {
                return [[UINavigationController alloc] initWithRootViewController:root];
            }
            constexpr auto& navctrl = navigationController;
            constexpr auto& navctl = navigationController;
            // 添加 UIImageView
            inline UIImageView* imageView(UIImage* image) {
                UIImageView* imageView = [[UIImageView alloc] initWithImage:image];
                imageView.contentMode = UIViewContentModeScaleAspectFit;
                imageView.translatesAutoresizingMaskIntoConstraints = NO;
                return imageView;
            }

            inline UIImageView* imageView(NSString* imageName) {
                UIImage* image = [UIImage imageNamed:imageName];
                return imageView(image);
            }
            auto imgview = [](auto&& arg) { return imageView(std::forward<decltype(arg)>(arg)); };
            // 添加 UITextField
            inline UITextField* textField(NSString* placeholder = nil) {
                UITextField* textField = [[UITextField alloc] init];
                textField.borderStyle = UITextBorderStyleRoundedRect;
                textField.placeholder = placeholder;
                textField.translatesAutoresizingMaskIntoConstraints = NO;
                return textField;
            }
            constexpr auto& textfield = textField;
            // 添加 UISwitch
            inline UISwitch* toggle() {
                UISwitch* toggle = [[UISwitch alloc] init];
                toggle.translatesAutoresizingMaskIntoConstraints = NO;
                return toggle;
            }
            // 添加 UIScrollView
            inline UIScrollView* scrollView() {
                UIScrollView* scrollView = [[UIScrollView alloc] init];
                scrollView.translatesAutoresizingMaskIntoConstraints = NO;
                return scrollView;
            }
            constexpr auto& scrollview = scrollView;
            // 添加 UIStackView
            inline UIStackView* stackView(NSArray* arrangedSubviews = nil) {
                UIStackView* stackView = [[UIStackView alloc] initWithArrangedSubviews:arrangedSubviews];
                stackView.axis = UILayoutConstraintAxisVertical;
                stackView.spacing = 10;
                stackView.distribution = UIStackViewDistributionFill;
                stackView.alignment = UIStackViewAlignmentCenter;
                stackView.translatesAutoresizingMaskIntoConstraints = NO;
                return stackView;
            }
            constexpr auto& stackview = stackView;
        } // namespace make
        namespace mk = make;
        // MARK: 自動佈局輔助
        namespace layout {
            inline void center(UIView* view, UIView* inView) {
                view.translatesAutoresizingMaskIntoConstraints = NO;
                NSMutableArray* constraints = [NSMutableArray array];
                // 置中約束
                [constraints addObject:[view.centerXAnchor constraintEqualToAnchor:inView.centerXAnchor]];
                [constraints addObject:[view.centerYAnchor constraintEqualToAnchor:inView.centerYAnchor]];
                // 對 UILabel 的特殊處理
                if ([view isKindOfClass:[UILabel class]]) {
                    UILabel* label = (UILabel*)view;
                    label.numberOfLines = 0;
                    label.textAlignment = NSTextAlignmentCenter;
                    [label setContentCompressionResistancePriority:UILayoutPriorityRequired 
                                                           forAxis:UILayoutConstraintAxisVertical];
                    [label setContentHuggingPriority:UILayoutPriorityRequired 
                                             forAxis:UILayoutConstraintAxisVertical];
                     // 添加邊距約束, 確保文字不超出螢幕
                    [constraints addObject:[label.leadingAnchor constraintGreaterThanOrEqualToAnchor:inView.leadingAnchor constant:20]];
                    [constraints addObject:[label.trailingAnchor constraintLessThanOrEqualToAnchor:inView.trailingAnchor constant:-20]];
                    [constraints addObject:[label.widthAnchor constraintLessThanOrEqualToAnchor:inView.widthAnchor constant:-40]];
                }
                [NSLayoutConstraint activateConstraints:constraints];
            }
            // 添加一個增強版的 center 函數, 允許自定義邊距
            inline void center(UIView* view, UIView* inView, CGFloat horizontalPadding, CGFloat verticalPadding) {
                view.translatesAutoresizingMaskIntoConstraints = NO;
                NSMutableArray* constraints = [NSMutableArray arrayWithArray:@[
                    [view.centerXAnchor constraintEqualToAnchor:inView.centerXAnchor],
                    [view.centerYAnchor constraintEqualToAnchor:inView.centerYAnchor],
                    [view.widthAnchor constraintLessThanOrEqualToAnchor:inView.widthAnchor constant:-horizontalPadding * 2]
                ]];
                // UILabel 特殊处理
                if ([view isKindOfClass:[UILabel class]]) {
                    UILabel* label = (UILabel*)view;
                    label.numberOfLines = 0;
                    label.textAlignment = NSTextAlignmentCenter;
                    [label setContentCompressionResistancePriority:UILayoutPriorityRequired 
                                                           forAxis:UILayoutConstraintAxisVertical];
                    [label setContentHuggingPriority:UILayoutPriorityRequired 
                                             forAxis:UILayoutConstraintAxisVertical];
/*
                    [label setContentCompressionResistancePriority:UILayoutPriorityRequired 
                                                           forAxis:UILayoutConstraintAxisHorizontal];
                    [label setContentHuggingPriority:UILayoutPriorityRequired 
                                             forAxis:UILayoutConstraintAxisHorizontal];
*/
                }

                [NSLayoutConstraint activateConstraints:constraints];
            }

            inline void fill(UIView* view, UIView* inView, CGFloat padding = 0) {
                [NSLayoutConstraint activateConstraints:@[
                    [view.topAnchor constraintEqualToAnchor:inView.topAnchor constant:padding],
                    [view.bottomAnchor constraintEqualToAnchor:inView.bottomAnchor constant:-padding],
                    [view.leadingAnchor constraintEqualToAnchor:inView.leadingAnchor constant:padding],
                    [view.trailingAnchor constraintEqualToAnchor:inView.trailingAnchor constant:-padding]
                ]];
            }
            // 垂直排列
            inline void vStack(NSArray* views, UIView* inView, CGFloat spacing = 8) {
                UIView* previousView = nil;
                for (UIView* view in views) {
                    view.translatesAutoresizingMaskIntoConstraints = NO;
                    [NSLayoutConstraint activateConstraints:@[
                        [view.centerXAnchor constraintEqualToAnchor:inView.centerXAnchor]
                    ]];

                    if (previousView) {
                        [view.topAnchor constraintEqualToAnchor:previousView.bottomAnchor 
                                                       constant:spacing].active = YES;
                    } else {
                        [view.topAnchor constraintEqualToAnchor:inView.topAnchor 
                                                       constant:spacing].active = YES;
                    }
                    previousView = view;
                }
                if (previousView) {
                    [previousView.bottomAnchor constraintLessThanOrEqualToAnchor:inView.bottomAnchor 
                                                                         constant:-spacing].active = YES;
                }
            }
            constexpr auto& vstack = vStack;
            // 添加約束捷徑
            inline void width(UIView* view, CGFloat width) { [view.widthAnchor constraintEqualToConstant:width].active = YES; }

            inline void height(UIView* view, CGFloat height) { [view.heightAnchor constraintEqualToConstant:height].active = YES; }

            inline void size(UIView* view, CGFloat width, CGFloat height) {
                [NSLayoutConstraint activateConstraints:@[
                    [view.widthAnchor constraintEqualToConstant:width],
                    [view.heightAnchor constraintEqualToConstant:height]
                ]];
            }
        } // namespace layout
        // MARK: 顏色輔助
        namespace color {
            inline UIColor* white() { return UIColor.whiteColor; }
            inline UIColor* black() { return UIColor.blackColor; }
            inline UIColor* red() { return UIColor.redColor; }
            inline UIColor* system_blue() { return UIColor.systemBlueColor; }
            inline UIColor* green() { return UIColor.greenColor; }
            inline UIColor* yellow() { return UIColor.yellowColor; }
            inline UIColor* gray() { return UIColor.grayColor; }
            inline UIColor* system_red() { return UIColor.systemRedColor; }
            inline UIColor* system_green() { return UIColor.systemGreenColor; }
            inline UIColor* system_background() { return UIColor.systemBackgroundColor; }
            constexpr auto& sysblue = system_blue;
            constexpr auto& sysred = system_red;
            constexpr auto& sysgreen = system_green;
            constexpr auto& sysbackground = system_background;
            // 自定義 RGB 顏色
            inline UIColor* rgb(CGFloat r, CGFloat g, CGFloat b) {
                return [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1.0];
            }

            inline UIColor* rgba(CGFloat r, CGFloat g, CGFloat b, CGFloat a) {
                return [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:a];
            }
        } // namespace color
        namespace font {
            inline UIFont* system(CGFloat size) { return [UIFont systemFontOfSize:size]; }

            inline UIFont* bold_system(CGFloat size) { return [UIFont boldSystemFontOfSize:size]; }
            constexpr auto& boldsys = bold_system;

            inline UIFont* italic_system(CGFloat size) { return [UIFont italicSystemFontOfSize:size]; }
            constexpr auto& italicsys = italic_system;

            inline UIFont* monospaced(CGFloat size) { return [UIFont monospacedDigitSystemFontOfSize:size weight:UIFontWeightRegular]; }
        } // namespace font
        namespace image {
            inline UIImage* system(NSString* name) {
                if (@available(iOS 13.0, *)) { return [UIImage systemImageNamed:name]; }
                return nil;
            }
    
            inline UIImage* named(NSString* name) { return [UIImage imageNamed:name]; }
        } // namespace image
        namespace img = image;

        namespace alert {
            inline void show(UIViewController* vc, NSString* title, NSString* message) {
                UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                               message:message
                                                                preferredStyle:UIAlertControllerStyleAlert];
                UIAlertAction* ok = [UIAlertAction actionWithTitle:@"OK"
                                                             style:UIAlertActionStyleDefault
                                                           handler:nil];
                [alert addAction:ok];
                [vc presentViewController:alert animated:YES completion:nil];
            }

            inline void confirm(UIViewController* vc, NSString* title, NSString* message, 
                                void (^completion)(BOOL confirmed)) {
                UIAlertController* alert = [UIAlertController alertControllerWithTitle:title
                                                                               message:message
                                                                preferredStyle:UIAlertControllerStyleAlert];
                UIAlertAction* cancel = [UIAlertAction actionWithTitle:@"取消"
                                                                 style:UIAlertActionStyleCancel
                                                               handler:^(UIAlertAction* action) {
                    if (completion) completion(NO);
                }];
                UIAlertAction* confirm = [UIAlertAction actionWithTitle:@"確定"
                                                                  style:UIAlertActionStyleDefault
                                                                handler:^(UIAlertAction* action) {
                    if (completion) completion(YES);
                }];
                [alert addAction:cancel];
                [alert addAction:confirm];
                [vc presentViewController:alert animated:YES completion:nil];
            }
        } // namespace alert
    } // namespace ui
    namespace UI = ui;
  # endif // ad_use_objc_code || ad_use_objc
} // namespace AD
# endif // _ADLIBCXX_OBJC_HPP_
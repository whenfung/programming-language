# 博客搭建

jekyll 是 github 官方支持的静态网站生成工具，一个极佳的前端学习工具和博客搭建工具。

jekyll 特别好学，就是一个框架，框架下的每个文件有特定的功能，你把功能实现后，jekyll 将目录打包成一个静态文件发布到站点上，而且 github 提供了仓库存放你的静态文件，这样你也不需要购买服务器了。

## 搭建流程

- [github 账号](https://github.com)

  如果你不了解 github，自行百度。

- [git 软件](https://git-scm.com/)

  github 提供了仓库存放你的文件，而 git 可以将你的文件从本地传到 github 的仓库上，git 使用可以参考[廖雪峰的 git 教程](https://www.liaoxuefeng.com/)。

- [ruby 下载源](https://rubyinstaller.org/downloads/)

  本人的电脑是 win10，如果你也是 win10，遇到了问题欢迎咨询。

- [域名](https://cloud.tencent.com/) (可选)

  github 会提供一个叫 usename.github.io 的域名给你，如果你嫌不好听，可以花点小钱购买一个好听的域名。

- [jekyll 教程](https://jekyllrb.com/)

  github 的网站渲染是基于 jekyll 的，所以你要学如何利用 jekyll 搭建一个框架，你可以结合 [jekyll 入门项目](https://github.com/professordeng/blog)学习。当然，如果你想舒服点，可以找现成的[主题模板](http://jekyllthemes.org/)。

## 元素风格

博客的核心功能就是写作，所以我们的首要任务是实现排版。下面是我的元素风格和排版，模仿 github 风格，并自带数学公式渲染。

### 图片
![图标](/img/example.jpg)

### 引用
> 垂死病中惊坐起，笑问客从何处来 -- 李白

### 代码块
``` c++
#include <iostream>

using namespace std;

int main(){
    cout << "Hello World!";
    return 0;
}
```



### 行间字体

- **粗体字**
- *斜体字*
- `highlight`
- ~~删除字~~
- <u>下划线</u>



### 表格

| 姓名 | 学号 | 成绩 |
| ---- | ---- | ---- |
| 张三 | 1    | 98   |
| 李四 | 2    | 99   |
| 风哥 | 3    | 101  |

## 公式

书写公式的时候，需要在文章最后加上 `{{site.math}}` 表示有公式需要渲染，而且由于 markdown 的 `|` 是有特殊含义的，为了避免公式里的 `|` 被 markdown 解释器渲染，可以用 `\mid` 代替 `|`。
$$
\frac{\mathrm{d}}{\mathrm{d}t} \left ( \frac {\partial L}{\partial \dot{q}_j} \right ) = \frac {\partial L}{\partial q_j}
$$

### 视频

<video src="https://cdn-video.xinpianchang.com/5b7fc02a84108.mp4" width = "100%" controls="" preload=""></video>

{{site.math}}
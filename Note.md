git init
git add <filename>
git commit -m "msg"
git clone <url>
git status 查看哪些文件处于什么状态
git add 会开始跟踪一个文件或者把已跟踪的文件放在暂存区，还能用于合并时把有冲突的文件标记为已解决状态

git status -s 或者 git status --short获得格式更加紧凑的输出
新添加的未跟踪文件前面有 ?? 标记,新添加到暂存区中的文件前面有 A 标记,修改过的文件前面有 M 标记。

文件 .gitignore 的格式规范如下:
• 所有空行或者以 # 开头的行都会被 Git 忽略。
• 可以使用标准的 glob 模式匹配,它会递归地应用在整个工作区中。
• 匹配模式可以以(/)开头防止递归。
• 匹配模式可以以(/)结尾指定目录。
• 要忽略指定模式以外的文件或目录,可以在模式前加上叹号(!)取反。
所谓的 glob 模式是指 shell 所使用的简化了的正则表达式。 星号(\*)匹配零个或多个任意字符;[abc] 匹配任何一个列在方括号中的字符 (这个例子要么匹配一个 a,要么匹配一个 b,要么匹配一个 c); 问号(?)只匹配一个任意字符;如果在方括号中使用短划线分隔两个字符, 表示所有在这两个字符范围内的都可以匹配(比如 [0-9] 表示匹配所有 0 到 9 的数字)。 使用两个星号(\**)表示匹配任意中间目录,比如 a/**/z 可以匹配 a/z 、 a/b/z 或 a/b/c/z 等。

git diff
此命令比较的是工作目录中当前文件和暂存区域快照之间的差异。 也就是修改之后还没有暂存起来的变化内容

git diff --staged 命令。 这条命令将比对已暂存文件与最后一次提交的文件差异

git diff --cached 查看已经暂存起来的变化

git commit 会启动所选择的文本编辑器来输入提交说明
git config --global core.editor 命令设置你喜欢的编辑器。

git commit -a -m 'added new benchmarks'
在提交的时候,给 git commit 加上 -a 选项,Git 就会自动把所有已经跟踪过的文件暂存起来一并提交,从而跳过 git add 步骤

git rm <file> 将文件从暂存区和工作区中删除
git rm -f <file>如果删除之前修改过并且已经放到暂存区域的话，则必须要用强制删除选项 -f \
git rm --cached <file> 文件从暂存区域移除，但仍然希望保留在当前工作目录中，换句话说，仅是从跟踪清单中删除
git rm log/\*.log  git rm 命令后面可以列出文件或者目录的名字,也可以使用 glob 模式.注意要使用'\'

git mv <file_from> <file_to> 可以用来rename
eg: git mv README.md README 等价于mv README.md README，git rm README.md，git add README三个命令

git log 会按时间先后顺序列出所有的提交,最近的更新排在最上面
-p 或 --patch ,它会显示每次提交所引入的差异(按 补丁 的格式输出)。 你也可
以限制显示的日志条目数量,例如使用 -2 选项来只显示最近的两次提交:
git log --status 可以看到每次提交的简略统计信息
git log --pretty 这个选项可以使用不同于默认格式的方式展示提交历史
git log --pretty=format:"%h - %an, %ar : %s"
git log --pretty=format:"%h %s" --graph
--oneline
--since 和 --until 这种按照时间作限制的选项很有用
-author 选项显示指定作者的提交,用 --grep 选项搜索提交说明中的关键字。
如果你添加了 --all-match 选项, 则只会输出匹配 所有--grep 模式的提交。
-S 它接受一个字符串参数,并且只会显示那些添加或删除了该字符串的提交
git log后面加上路径可以查看某个文件的历史

git commit --amend有时候我们提交完了才发现漏掉了几个文件没有添加,或者提交信息写错了。 此时,可以运行带有 --amend 选项的提交命令来重新提交:

git reset HEAD <file>取消暂存
下面两个命令
git reset --hard origin/master
git reset --hard HEAD 
git checkout -- <file> 是一个危险的命令。 你对那个文件在本地的任何修改都会消失——Git 会用最近提交的版本覆盖掉它

git remote 
git remote -v
git remote add <shortname> <url>

git fetch <remote>访问远程仓库,从中拉取所有你还没有的数据。 执行完成后,你将会拥有那个远程仓库中所有分支的引用,可以随时合并或查看

git pull 等价于 git fetch and git merge

git push <remote> <branch>
只有当你有所克隆服务器的写入权限,并且之前没有人推送过时,这条命令才能生效。 当你和其他人在同一时间克隆,他们先推送到上游然后你再推送到上游,你的推送就会毫无疑问地被拒绝。 你必须先抓取他们的工作并将其合并进你的工作后才能推送。

git remote show <remote>

git remote rename  <old remote name> <new remote name>
git remote rm or git remote remove 移除一个远程仓库
git push origin --delete branch,该指令也会删除追踪分支

git tag 列出所有的标签
git tag -l "v1.8.5*" 里面是匹配表达式

轻量标签很像一个不会改变的分支——它只是某个特定提交的引用。
附注标签是存储在 Git 数据库中的一个完整对象, 它们是可以被校验的,其中包含打标签者的名字、电子邮件地址、日期时间, 此外还有一个标签信息,并且可以使用 GNU Privacy Guard (GPG)签名并验证。

以下是附注标签
git tag -a v1.4 -m "my version 1.4"
-m 选项指定了一条将会存储在标签中的信息
git show v1.4

以下是轻量标签
git tag <tag_name>
时,如果在标签上运行 git show,你不会看到额外的标签信息。 命令只会显示出提交信息

git tag -a <tag_name> <check_sum> 之后补上标签，需要在命令最后指定校验和或者部分检验和

默认情况下,git push 命令并不会传送标签到远程仓库服务器上。 在创建完标签后你必须显式地推送标签到共享服务器上。 这个过程就像共享远程分支一样——你可以运行 git push origin <tagname>。

如果想要一次性推送很多标签,也可以使用带有 --tags 选项的 git push 命令。 这将会把所有不在远程仓库服务器上的标签全部传送到那里。 git push origin --tags

git tag -d <tagname> 删除掉你本地仓库上的标签
git push <remote> :refs/tags/<tagname>更新你的远程仓库那个标签
git push origin --delete <tagname>

git 起别名提高效率 
git config --global alias.co checkout

当使用 git commit 进行提交操作时,Git 会先计算每一个子目录(本例中只有项目根目录)的校验和, 然后在 Git 仓库中这些校验和保存为树对象。随后,Git 便会创建一个提交对象, 它除了包含上面提到的那些信息外,还包含指向这个树对象(项目根目录)的指针。 如此一来,Git 就可以在需要的时候重现此次保存的快照。

提交对象指向树对象，树对象指向blob对象

提交的对象会指向以前的提交对象，一直指到最开始

git branch <branch_name> 创建分支
HEAD是一个指针,指向当前所在的本地分支(译注:将 HEAD 想象为当前分支的别名)
git checkout <branch_name> 切换分支
以上两个命令可以为一个 git checkout -b <branch_name>

git branch -d <branch_name> 删除本地分支 -D强制删除

git merge <branch_name> merge指定分支到目前分支
遇到冲突
HEAD 所指示的版本(也就是你的 master 分支所在的位置,因为你在运行 merge 命令的时候已经检出到了这个分支)在这个区段的上半部分(======= 的上半部分),而 iss53 分支所指示的版本======= 的下半部分。 为了解决冲突,你必须选择使用由 ======= 分割的两部分中的一个,或者你也可以自行合并这些内容。
解决完冲突后，git add 这个冲突文件，git就会把他们标记为冲突已解决

git branch 查看所有分支
git branch -v 查看每一个分支的最后一次提交,可以运行 
--merged 与 --no-merged 这两个有用的选项可以过滤这个列表中已经合并或尚未合并到当前分支的分支

git ls-remote <remote> 来显式地获得远程引用的完整列表
git remote show <remote> 获得远程分支的更多信息

跟踪分支
从一个远程跟踪分支检出一个本地分支会自动创建所谓的“跟踪分支”(它跟踪的分支叫做“上游分支”)。跟踪分支是与远程分支有直接关系的本地分支。 如果在一个跟踪分支上输入 git pull,Git 能自动地识别去哪个服务器上抓取、合并到哪个分支。
git checkout -b <branch> <remote>/<branch>
git checkout --track origin/serverfix 如果你尝试检出的分支 (a) 不存在且 (b) 刚好只有一个名字与之匹配的远程分支,那么 Git 就会为你创建一个跟踪分支:

git branch -u orgin/serverfix
设置已有的本地分支跟踪一个刚刚拉取下来的远程分支,或者想要修改正在跟踪的上游分支, 你可以在任意时间使用 -u 或 --set-upstream-to 选项运行 git branch 来显式地设置。

git merge <remote>/<branch> 将远程仓库上的分支合并到当前的分支

git branch -vv 这会将所有的本地分支列出来并且包含更多的信息,如每一个分支正在跟踪哪个远程分支与本地分支是否是领先、落后或是都有

it push origin --delete serverfix 删除远程仓库的分支

rebase 命令将提交到某一分支上的所有修改都移至另一分支上
merge是从别的分支merge到当前，rebase是把当前的修改移到指定分支的末尾。
它的原理是首先找到这两个分支(即当前分支 experiment、变基操作的目标基底分支 master) 的最近共同祖先 C2,然后对比当前分支相对于该祖先的历次提交,提取相应的修改并存为临时文件, 然后将当前分支指向目标基底 C3, 最后以此将之前另存为临时文件的修改依序应用。可以快速合并

两种整合方法的最终结果没有任何区别,但是变基使得提交历史更加整洁。 你在查看一个经过变基的分支的历史记录时会发现,尽管实际的开发工作是并行的, 但它们看上去就像是串行的一样,提交历史是一条直线没有分叉

变基是将一系列提交按照原有次序依次应用到另一分支上,而合并是把最终结果合在一起。

先在自己的分支里进行开发,当开发完成时你需要先将你的代码变基到origin/master 上,然后再向主项目提交修改。 这样的话,该项目的维护者就不再需要进行整合工作,只需要快进合并便可.

git rebase master server 把server rebase到master上
git rebase --onto master server client 
取出 client 分支,找出它从 server 分支分歧之后的补丁, 然后把这些补丁在master 分支上重放一遍,让 client 看起来像直接基于 master 修改一样。这样server上的修改就不会合并到matser上

git push -f 强制推送

rebase出现尴尬的处境 使用git pull --rebase或者 先 git fetch,再 git rebase teamone/master

git rev-parse topic1 查看这个别名代表的sha-1
git reflog
git show HEAD@{0}

git show master@{yesterday} 显示昨天 master 分支的顶端指向了哪个提交。 这个方法只对还在你引用日志里的数据有用,所以不能用来查好几个月之前的提交。

git log -g master

祖先引用是另一种指明一个提交的方式。 如果你在引用的尾部加上一个 ^(脱字符), Git 会将其解析为该引用的上一个提交
例如 git log HEAD^

你也可以在 ^ 后面添加一个数字来指明想要 哪一个 父提交

另一种指明祖先提交的方法是 \~(波浪号)。 同样是指向第一父提交,因此 HEAD~ 和 HEAD^ 是等价的。 而区别在于你在后面加数字的时候。 HEAD~2 代表“第一父提交的第一父提交”,也就是“祖父提交”——Git 会根据你指定的次数获取对应的第一父提交

..表示区间
experiment..master 会显示在 master 分支中而不在 experiment 分支中的提交

stash 贮藏
想要切换分支,但是还不想要提交之前的工作;所以贮藏修改
执行git stash 或者 git stash push

git restore 


$ git cherry-pick feature
# 上面代码表示将feature分支的最近一次提交，转移到当前分支。
git cherry-pick 可以pick对应的hash码，把对应的提交（仅限那次提交版本）转移到当前分支后面
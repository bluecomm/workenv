# ----- config -----
## ----- init -----
git init
git clone <url>
git config --global user.name "xxx"
git config --global user.email "xxx@xxx.com"

## ----- alias -----
git config --global --add alias.st status
git config --global --add alias.co checkout
git config --global --add alias.br branch
git config --global --add color.ui true
git config --global --add alias.mpush "push origin HEAD:refs/for/master"
git config --global --add alias.acommit "commit --amend --no-edit"
git config --global --list
"""
user.email=memo4096@163.com
user.name=majing
alias.st=status
alias.co=checkout
alias.br=branch
color.ui=true
color.branch=true
color.diff=true
color.status=true
"""

## ----- remember password when push github -----
git config --global credential.helper store

# ----- modify and commit -----
git add
git add -u
git commit
commit --amend --no-edit
git push origin HEAD:refs/for/master

# ----- log -----
git log
git log -p <file>
git log -2
git log --reflog
git blame filename
git reflog
git log dev..master #查看master有但dev没有的提交
git log dev...master # 查看master或dev的提交

# ----- diff -----
git diff
git diff commit1 commit2
git diff --stat  # simple diff result(only changed filename)
git show <commit-id> # show diff of this commit

# ----- cherry-pick changes to branch -----
git checkout dev
git cherry-pick 508bd350553ca53616d3b879c4da1aec7c5cf819

# ----- discasrd local change / undo -----
git fetch origin master
git reset --soft origin/master
git reset --hard origin/master
git checkout -- <file>
git checkout HEAD <file>
git reset --soft origin/master
git reset --hard origin/master
git revert <commit>
git cherry-pick <commit>

# ----- branch -----
## ----- delete branch/tag -----
git push origin --delete <branchName>
git push origin:<branchName>

# ----- tag -----
git ls-remote --tags origin
git tag
git tag <tagname>
git tag -l 
git tag -d <tagname>
git tag -a -m "comment for tag"
git tag -a <commitid>
git push origin --tags
git push origin <tagname>
git push origin --delete tag <tagname>

# ----- git work-flow -----
git checkout master
git pull origin master
git checkout -b <my branch name>
...
git add -u
git commit
git checkout master
git pull origin master
git checkout <my branch name>
git rebase -i master
git push

# ----- set git color -----
git config --global color.ui true
git config --global color.ui false

## color content
color.branch
color.diff
color.interactive
color.status

eg. git config --global color.diff.meta "blue black bold"

## color choice
normal
black
red
green
yellow
blue
magenta
cyan
white

## meta choice
bold
dim
ul
blink
reverse

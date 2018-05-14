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

## ----- remember password when push github -----
git config --global credential.helper store

# ----- modify and commit -----
git add
git add -u
git commit
git push origin HEAD:refs/for/master

# ----- log -----
git log
git log -p <file>
git log -2
git log --reflog
git blame filename
git reflog
git log dev..master #�鿴master�е�devû�е��ύ
git log dev...master # �鿴master��dev���ύ

# ----- diff -----
git diff
git diff commit1 commit2
git diff --stat  # simple diff result(only changed filename)

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
git push origin :<branchName>
git push origin --delete tag <tagname>

# ----- git work-flow -----
git checkout work
...
git add -u
git commit
git checkout master
git pull
git checkout work
git rebase master
git checkout master
git merge work
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
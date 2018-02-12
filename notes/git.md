# ----- init -----
git init
git clone <url>
git config --global user.name "xxx"
git config --global user.email "xxx@xxx.com"

#----- modify and commit -----
git add
git add -u
git commit
git push origin HEAD:refs/for/master

# discasrd a local commit
git fetch origin master
git reset --soft origin/master

# cherry-pick changes to branch
git checkout updater_1-0-178_BRANCH
git cherry-pick 508bd350553ca53616d3b879c4da1aec7c5cf819
git push origin HEAD:refs/for/updater_1-0-178_BRANCH

# ----- log -----
git log
git log -p <file>
git log -2
git log --reflog
git blame filename
git reflog
git log dev..master #查看master有但dev没有的提交
git log dev...master # 查看master或dev的提交

# ----- undo -----
git checkout -- <file>
git checkout HEAD <file>
git reset --soft origin/master
git reset --hard origin/master
git revert <commit>
git cherry-pick <commit>

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

# ----- remember password when push github -----
git config --global credential.helper store

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

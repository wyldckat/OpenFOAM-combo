Introduction
============

This OpenFOAM-combo repository was originally started by following the instructions written by Mark Olsen here: [Merging OpenFOAM versions in git](http://olesenm.github.io/2009/11/24/merging-OpenFOAM-versions/)

The objective is to have a single repository where all official OpenFOAM git repositories are tracked in a single repository, since this allows one to make it easier to keep track of changes made between versions.

Nonetheless, further changes were needed to those instructions, since they did not take into account a proper adaptation from version to version and were only a quick way to merge all of the history into a single repository. Reference sources for the changes made to the original instructions:
 * How to use `git replace` for stitching up the connections between two unconnected branches: http://stackoverflow.com/a/19860058

*Disclaimer*: This offering is not approved or endorsed by OpenCFD Limited, the producer of the OpenFOAM software and owner of the OPENFOAM®  and OpenCFD® trade marks.

How to use
==========

In order to only browse online, have a look into the [`combo` branch](https://github.com/wyldckat/OpenFOAM-combo/tree/combo).

In order to have your own local copy, run these commands:
```
git clone https://github.com/wyldckat/OpenFOAM-combo.git
cd OpenFOAM-combo
git checkout combo
```

Now, locally, if you want to checkout how `simpleFoam` has evolved over the versions, you can use `gitk` like this:
```
gitk applications/solvers/incompressible/simpleFoam/
```

Creating a repository similar to this one
=========================================

*Note:* The repository for 1.6.x is omitted because the full history from 1.6.x is preserved in 1.7.x.

In a nutshell, run these commands:
```
cd ~
cd OpenFOAM
mkdir OpenFOAM-combo
cd OpenFOAM-combo/

git init
git remote add of15x git://repo.or.cz/OpenFOAM-1.5.x.git
git remote add of17x git://github.com/OpenCFD/OpenFOAM-1.7.x
git remote add of20x git://github.com/OpenFOAM/OpenFOAM-2.0.x.git
git remote add of21x git://github.com/OpenFOAM/OpenFOAM-2.1.x.git
git remote add of22x git://github.com/OpenFOAM/OpenFOAM-2.2.x.git

git remote | while read line; do git fetch $line; done

#create a README file and commit it.

git checkout -b master15x of15x/master
git checkout -b master17x of17x/master
git checkout -b master20x of20x/master
git checkout -b master21x of21x/master
git checkout -b master22x of22x/master

git branch -m master15x combo
```

Here things get a bit tricky:
 1. First we need to checkout the next version on the list;
 2. then go back to the first commit on that branch;
 3. tag that commit.
 4. Then checkout the previous version.
 5. Remove all files with git, except the `.git` folder.
 6. Then checkout all of the files from the first commit on the next version.
 7. Commit the changes, using the original commit message for the next version.
 8. And `git replace` the first commit on the next version for this lastest commit.
 9. Now switch to the next version and rebase towards the previous version.
 10. Now switch to the previous version and merge with the next version.
 11. And it's done for this group. Now go back to the start of this list and do again for the next version and this one.

Code-wise, for 1.5.x->1.7.x:

```
git checkout combo
git tag 15x-end
git checkout master17x
git checkout $(git rev-list --max-parents=0 HEAD)
git tag 17x-start
git checkout combo
git rm -rf * .gitignore
git checkout 17x-start -- .
git commit -c 17x-start

git replace 17x-start HEAD
git checkout master17x
git rebase combo
git checkout combo
git merge master17x
git branch -D master17x
```

The full code is actually this:

```
versionA=15x
versionB=17x

git checkout combo
git tag $versionA-end
git checkout master$versionB
git checkout $(git rev-list --max-parents=0 HEAD)
git tag $versionB-start
git checkout combo
git rm -rf * .gitignore
git checkout $versionB-start -- .
git commit -c $versionB-start

git replace $versionB-start HEAD
git checkout master$versionB
git rebase combo

#needed to manually repair the merge a few times, by using:
git mergetool
git rebase --continue

#once the rebase is complete:
git checkout combo
git merge master$versionB
git branch -D master$versionB


versionA=17x
versionB=20x

git checkout combo
git tag $versionA-end
git checkout master$versionB
git checkout $(git rev-list --max-parents=0 HEAD)
git tag $versionB-start
git checkout combo
git rm -rf * .gitignore
rm -rf *
git checkout $versionB-start -- .
git commit -c $versionB-start

git replace $versionB-start HEAD
git checkout master$versionB
git rebase combo

#needed to manually repair the merge a few times, by using:
git mergetool
git rebase --continue

git checkout combo
git merge master$versionB
git branch -D master$versionB


versionA=20x
versionB=21x

git checkout combo
git tag $versionA-end
git checkout master$versionB
git checkout $(git rev-list --max-parents=0 HEAD)
git tag $versionB-start
git checkout combo
git rm -rf * .gitignore
rm -rf *
git checkout $versionB-start -- .
git commit -c $versionB-start

git replace $versionB-start HEAD
git checkout master$versionB
git rebase combo

git checkout combo
git merge master$versionB
git branch -D master$versionB


versionA=21x
versionB=22x

git checkout combo
git tag $versionA-end
git checkout master$versionB
git checkout $(git rev-list --max-parents=0 HEAD)
git tag $versionB-start
git checkout combo
git rm -rf * .gitignore
rm -rf *
git checkout $versionB-start -- .
git commit -c $versionB-start

git replace $versionB-start HEAD
git checkout master$versionB
git rebase combo

#needed to manually repair the merge a few times, by using:
git mergetool
git rebase --continue

git checkout combo
git merge master$versionB
git branch -D master$versionB
```

As for tags, those have to be reconstructed manually, because this rebasing strategy will redo all git commits!


Updating the combo repository
=============================

WARNING: I've messed up the commit tree... might need to start over :(

This example is for the transition between OpenFOAM 2.2.x to 2.3.x.
First, use `gitk --all` to see where the `combo` branch stopped at.
Then run:
```
git checkout -b master22x of22x/master
git pull
gitk
```

Search for the commit that corresponds to the last commit on the combo branch and get the one next to it. In my case, it was `f427c14b50f589d1e5d698c893c23d75685cfe74`.
Quit `gitk` and do the following:
```
git checkout combo
git tag combo-end

git checkout master22x
git checkout f427c14b50f589d1e5d698c893c23d75685cfe74
git checkout -b referencePoint
git tag 22x-referencePoint
git merge master22x

git checkout combo
git rm -rf * .gitignore
rm -rf *
git checkout 22x-referencePoint -- .
git commit -c 22x-referencePoint

git replace 22x-referencePoint HEAD
git checkout referencePoint

#probably will have to use cherry-pick via gitk, at least for a few commits until a common merge, but this is very roughly what I did
git rev-list --reverse b6d5916abadc9d96544c409fbe0890c6cc9315ec..f9a78f7f7596e1fcac48e3a51c47e2091eda1b2a | xargs -n 1 git cherry-pick

git checkout combo
git tag of22x-end
git tag -d 22x-referencePoint
git branch -D master22x
git gc
```

As for hooking up to 2.3.x:
```
git remote add of23x git://github.com/OpenFOAM/OpenFOAM-2.3.x.git
git fetch of23x
git checkout -b master23x of23x/master

versionA=22x
versionB=23x

git checkout combo
git tag $versionA-end
git checkout master$versionB
git checkout $(git rev-list --max-parents=0 HEAD)
git tag $versionB-start
git checkout combo
git rm -rf * .gitignore
rm -rf *
git checkout $versionB-start -- .
git commit -c $versionB-start

git replace $versionB-start HEAD
git checkout master$versionB
git rebase combo

#needed to manually repair the merge a few times, by using:
git mergetool
git rebase --continue
#git rebase --skip

git checkout combo
git merge master$versionB
git branch -D master$versionB
git tag -d $versionB-start
```

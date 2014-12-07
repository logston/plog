---
layout: post
title: "Setting up a Vagrant development environment on OSX"
date: 2014-12-06 13:28:04
categories: code 
---

What I wanted
=============

I really like managing virtual machines with Vagrant. The tool is easy to use
and a real pleasure to play around with. I also really like my Mac. Thus, a 
good chunk of my development environments are built using Vagrant on my Mac. 
This article shares one wall I ran into while developing with Vagrant on OSX and 
how I climbed over that wall.

My Goal: 

I would like to serve my app from an environment that is as close to production 
(EC2 Linux box) as possible. However, I would like to edit my project's code 
with an IDE (PyCharm) installed on my host machine.

My Proposed Setup:

Share a directory on my Mac's local file system that contains all of the code 
for my project (ie. my project's repo) with the virtual machine (VM).  

The Problem
===========

When my Mac shares my project's repo directory with the VM, the VM doesn't 
preserve the cases of the letters within the names of the shared files[#]_. For
example,

    /Users/paul/shared_dir/App.py

would be seen as 

    /vagrant/app.py

on the VM, assuming that `/vagrant` was mapped to `/Users/paul/shared_dir`. 

I first noticed this issue when a series of `ImportError`\s (python) began 
popping up
while trying to run a management command associated with my Django project. 
I had a package named KISSmetrics installed (via pip)
and I also had a file named kissmetrics.py (written by my team)
in the projects repo. 

The following line was located in the `kissmetrics.py` file and would cause
the management command to implode when run.

    from KISSmetrics import KM

Now, I could have named `kissmetrics.py` something like 
`kissmetrics_utils.py` and solved the problem, but I was more interested in 
understanding why two files with different names (one capitalized and one not) 
were being interpreted as the same.

This issue especially puzzled me because I thought both 
Mac OSX (10.8 at the time) and Ubuntu (12.04) were,
by default, case sensitive. 

Investigation
=============

After some `investigation`_, I found out that the vagrant VM uses the same drive
formatting rules that the host uses for the shared directory. Thus, if
a directory `/Users/paul/shared_dir` is located on a drive that is formatted as
"Mac OS Extended (Journaled)" (ie. not case-sensitive) on the host machine, 
when that directory is shared with the VM as the directory `/vagrant/`  
within the VM, the `/vagrant/` directory will also not be case sensitive. 
And my host drive was formatted as "Mac OS Extended (Journaled)" rather than 
"Mac OS Extended (Case-sensitive, Journaled)".

So, to jump back to my original issue for a second, when Python tried to import
`KISSmetrics` it raised an `ImportError` because `kissmetrics.py` had already 
been imported. Python asked the underlying OS/disk for `KISSmetrics` and the 
OS/disk said "OK! Here you go" but handed back `kissmetrics.py` cause the OS/disk
knew no different. The disk didn't understand that it was being asked for a 
file with capital letters. 

At this point I had a diagnosis. The cure for my issue seemed to be reformatting
my host drive to be "Mac OS Extended (Case-sensitive, Journaled)". 
I wasn't about to do that. Instead, I decided to put my project onto a new 
volume which I would format to be case-sensitive. 

Below are details on how I created my case-sensitive drive. 

Solution
========

Ready Your Mac
--------------

- Install the following:

    - `Virtualbox <https://www.virtualbox.org/wiki/Downloads>`_
    - `Vagrant <https://www.vagrantup.com/downloads.html>`_

Create a drive
--------------

A case-sensitive drive needs to be created and shared with the vagrant VM. 

From within Disk Utility, click "New Image" to create a new drive. 
    
Create a drive with the following configuration:
        
    - Size: Custom (8 GB)
    - Format: Mac OS Extended (Case-Sensitive, Journaled)
    - Save as: <anything you want>.sparsebundle*
    - Name: <anything you want>*
    - Partitions: Single partition - GUID Partition Map
    - Encryption: None
    - Image Format: sparse bundle disk image

The value you give for "Save as" (ie. `<whatever you choose>.sparsebundle`) 
will be the name of the sparse bundle on your Mac's local file system. 
The value you give for "Name" will be the name of the mounted drive 
when you mount the sparse bundle. For example, a mounted drive with a name of 
"pauls_drive" will be located at `/Volumes/pauls_drive`.

     - Why Case-Sensitive - https://medium.com/@jasonsuave/a-more-useful-vagrant-38445d0d0736
     - Why Sparse Bundle - https://discussions.apple.com/thread/2001162?tstart=0
     - Why GUID partition - https://developer.apple.com/library/mac/technotes/tn2166/_index.html
     - Why Journaling - http://support.apple.com/en-us/HT2355

|disk-utils|

.. |disk-utils| image:: /media/2014-12-06-Vagrant-development-environment-on-OSX/disk_utils.png

.. _investigation: https://medium.com/@jasonsuave/a-more-useful-vagrant-38445d0d0736








can not import kissmetrics due to import 
error due to case insensitive vagrant machine



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

.. |disk-utils| image:: 

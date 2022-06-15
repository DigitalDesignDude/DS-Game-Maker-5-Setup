# DS Game Maker 5.20 Setup for Windows 11/10

![Thumbnail_SetupDsgmOnWindows](https://user-images.githubusercontent.com/106634287/172290113-be6cfb1b-8691-4e6e-a118-03230c9ca70b.png)


## About DS Game Maker

DS Game Maker (DSGM) is early 2000's software for developing Nintendo Ds homebrew games with basic drag-and-drop visual coding, with the C programming language or with it's own language called DBAS. It was Developed By James Garner and has been tested to still run on Windows 7, 10 and now 11 computers using this setup tutorial by DigitalDesignDude.
<br>
<br>
<br>



## 1st Step - DevkitPro Setup
  1. Download the devkitPro.zip file, extract it, and place it on the root of C drive of your computer. Thus the devkitPro folder's location will be "C:\devkitPro"
	
  2. If you don't have a C drive or wish to place devkit pro on the root of another drive you should be able to but you'll have to make sure to use the appropriate drive letter instead of "C" when creating the system variables later on in this tutorial. 
<br>
<br>



## 2nd Step - DS Game Maker Setup

  1. Download the DS Game Maker.zip file, extract it and place the "DS Game Maker" folder it anywhere you want on your PC. Such as in the Documents folder where it's easy to get to.
	
  2. Within the "DS Game Maker" folder make the "DS Game Maker.exe" file always  run in administrator mode by right clicking it then choosing "properties" from the right click menu options and then checking the "Run this program as an administrator" option from under the Compatibility tab in the file properties window.
<br>
<br>



## 3rd Step - Add the Proper PC Environment Variables DSGM needs

PC environment variables provide extra information that apps may need to in order to find other files or data on your computer and it would normally be added for you when you install a program using a program installer file.

Since the DSGM 5.20 installer that was released years ago installs a buggy version of DSGM 5.20 and doesn't add the proper environment variables we'll be adding them ourselves to the Windows 10/11 Environment Variables menu.
<br>
  1. Using the Windows 10 or 11 search bar search for "edit the system environment variables"  and click on that option.
  
  2.  When the new window opens click the "Environment Variables..." button to be brought to where you can edit or create the needed system environment variables below.

<br>
NOTE: the environment variables and values are case sensitive so they must be entered or pasted in exactly as shown below. If you installed devkitPro on a drive different than the C drive, then be sure to use the appropriate drive letter instead of "c" for the variables' values.
<br>
<br>
<br>
	
	System Variable: DEVKITPRO
	Value: /c/devkitPro
	
	System Variable: DEVKITARM
	Value: /c/devkitPro/devkitARM
	
	System Variable: Path
	Values Added:
	c:\devkitPro\msys\bin

	
	NOTE: Just add the above value to the "Path" Variable because it may contain multiple values already.
	
<br>
<br>

## LAST Step - Test Making a Nintendo DS Game file with DSGM
1. Run the DSGameMaker.exe and click "yes" if prompted by the windows administrator permission request popup.

2. Test compiling a blank project by pressing the "Test Game" button from the top ribbon of DSGM's interface (it looks like a play button).

Congrats! If you have no errors you're ready to start building Nintendo DS games with Ds Game Maker!
<br>
<br>
<br>

# DSGM Knowlegde Resources!
![banner_DsgmKnowlegdeResources](https://user-images.githubusercontent.com/106634287/173729565-1b842d40-cd44-4376-91d4-6eb6a5515957.png)
<br>
<br>


### DS Game Maker Example Files Coming Soon!  
Follow my [Itch Page](https://surrealcubemedia.itch.io/) for Updates.

### DS Game Maker Video Tutorials Coming Soon! 
Subscribe on [YouTube](https://www.youtube.com/channel/UCVmba1o66nWG6p-82g22iyg) to keep informed on when they are released.

### DS Game Maker Discord Servers
- [DS Game Maker](https://discord.gg/uzNM8gGSA4)
- [DSGM Online](https://discord.gg/HqbuAy9xJf)

### DS Game Maker Documentation
- [DS Game Maker 5 Manual.pdf](https://github.com/DigitalDesignDude/DS-Game-Maker-5.20-Setup/files/8905375/DS.Game.Maker.5.Manual.pdf)
- [DSGM Syntax at a Glance.pdf](https://github.com/DigitalDesignDude/DS-Game-Maker-5.20-Setup/files/8905386/DSGM.Syntax.at.a.Glance.pdf)
- [Index of DS Game Maker Functions.pdf](https://github.com/DigitalDesignDude/DS-Game-Maker-5.20-Setup/files/8905373/Index.of.DS.Game.Maker.Functions.pdf)
- [PAlib Functions Reference.pdf](https://github.com/DigitalDesignDude/DS-Game-Maker-5.20-Setup/files/8905406/PAlib.Functions.Reference.pdf)
- [Troubleshooting Common Errors in DSGM 5.pdf](https://github.com/DigitalDesignDude/DS-Game-Maker-5.20-Setup/files/8905369/Troubleshooting.Common.Errors.in.DSGM.5.pdf)



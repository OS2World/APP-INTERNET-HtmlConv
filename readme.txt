----------------------------------------------------------------------------
What HtmlConv does
----------------------------------------------------------------------------

Some developers of email clients now think it's cool to have their software send emails in text/html format by default. Many users are either not aware of this or don't know how to change this setting. Some others may even like it this way.

PMMail 1.x for OS/2 doesn't know about HTML. It displays the HTML code unmodified in the Read Message Window. Of course HTML source text is not easy to read.

HtmlConv is a converter for text/html emails. It converts a text/html email file into a multipart/alternative email file with a text/plain version of the original text/html email as its first part. This text/plain version can then be displayed by PMMail without problems.

HtmlConv has been written to be used with the filter feature of PMMail for OS/2. However, it may also be used with any other email client that can run an external program to modify incoming emails.

HtmlConv may also be used as a command line tool if the input file contains only one single email.

----------------------------------------------------------------------------
Other versions of PMMail
----------------------------------------------------------------------------

HtmlConv is a tool for PMMail 1.x for OS/2. You don't need HtmlConv, if you have one of the following versions of PMMail, since they are already able to handle text/html:

- PMMail/2 2.0 and newer
- PMMail98

----------------------------------------------------------------------------
This is inmportant!
----------------------------------------------------------------------------

HtmlConv is freeware. 

So, strictly speaking, you should not expect this program to do anything else than to decrease the amount of free memory in your computer. Since you don't pay anything for this program, this is actually already a bit of feature overkill... ;-)

Anyway, I appreciate any suggestions how to improve HtmlConv. If you find that HtmlConv doesn't work as you think it should, send me a note. If possible, please attach an example for an email that causes the problem.

----------------------------------------------------------------------------
The most recent version of HtmlConv
----------------------------------------------------------------------------

I will upload the most recent version of HtmlConv to Hobbes (http://hobbes.nmsu.edu/ or ftp://hobbes.nmsu.edu/) only.

I have no particular preferences for Hobbes, except that it's one of the better known OS/2 software servers in that part of the world where I presently happen to live.


----------------------------------------------------------------------------
Files in this distribution
----------------------------------------------------------------------------

In the htmlconv directory:

filter.bmp   - Example for PMMail filter settings
history      - Release history
liesmich     - Readme file for German speakers
mime.bmp     - Example for MIME Associations settings for text/html
readme       - This file
htmlconv.c   - The source code of HtmlConv
htmlconv.exe - The HtmlConv executable
todo         - List of missing features, workarounds, restrictions, etc

In the htmlconv\pgp directory:

history.sig  - My PGP signature for history
liesmich.sig - My PGP signature for liesmich
readme.sig   - My PGP signature for readme
htmlconv.sig - My PGP signature for htmlconv.c
todo.sig     - My PGP signature for todo

----------------------------------------------------------------------------
How to use HtmlConv
----------------------------------------------------------------------------

If you enter just "htmlconv" in a command line window, the program will output some helpful informations to the screen. 

With the proper PMMail filter set-up (see below) HtmlConv will convert any incoming text/html email to a multipart/alternative email that consists of two parts:

(1) A text/plain part:
This is a reduced-formatting version of the original text/html email. HTML tags are simply removed from the original text. This text/plain part will be automatically displayed in PMMail's Read Message Window.

(2) A text/html part:
This is the unmodified, original text/html email. You may view it by double clicking on the icon of this attachment, but you may have to change your PMMail settings before this works. See below for instructions how to do that.

To use HtmlConv you first need to put htmlconv.exe in a directory of your choice. This directory doesn't have to be listed in your PATH environvent variable, and it also doesn't have to be any of the PMMail directories.

Then you need to set up a filter in PMMail that calls HtmlConv. Here's how to do that:

(1) Go to Account->Account Settings...->Filters

In PMMail versions prior to 1.96 you may have to go to Account->Utilities Settings...->Filters instead.

(2) Click on New.

(3) Enter the following data in the Edit Filter window (see filter.bmp for an example):

  Desription  Any name you like
  Enabled     [should be checked]
  Simple
    Search    <Header> 
    For       Content-Type: text/html
  Type        Incoming
  Actions     REXX Exit (Background) x:\full\path\to\htmlconv.exe

You don't need to enter any arguments or %s after HtmlConv.exe. 

During the first time it's usually helpful to add the "Inform User" action, so you can immediately know when this filter finds a matching email and calls HtmlConv.

(4) Click on OK. 

(5) Make shure there is no filter that moves incoming text/html messages out of the Inbox, before HtmlConv had a chance to convert them. If necessary, use the Up/Down buttons to change the filter sequence.

(6) Make sure that there is no other conflict with another filter. That's the hardest part, and you're on your own... good luck ;-)

That's it.

----------------------------------------------------------------------------
How to view the text/plain part
----------------------------------------------------------------------------

The text/plain part should be displayed once you double-click on the message in PMMail's message list. (Actually that's the main purpose of HtmlConv.)

----------------------------------------------------------------------------
How to view the text/html part
----------------------------------------------------------------------------

You may still view the unaltered, original text/html part by double clicking on the second attachment. There are two ways how to do that:

(1) Just drag and drop the first attachment on your favorite WWW browser. 

(2) In case you haven't done this yet, go to PMMail->Settings->MIME Associations, klick on Add and enter the following data (see mime.bmp for an example):

  Description           HTML
  MIME Type             text / html
  File Extension        html htm
  Program To execute:   x:\path\to\your\favorite\browser.exe
  Arguments:            [other arguments if necessary] %s
  Working Directory     [leave this field empty]
  Program Type          [depends on your browser]

Now a double click on the first attachment should load it into your WWW browser.

----------------------------------------------------------------------------
How to test HtmlConv
----------------------------------------------------------------------------

HtmlConv reads a file, modifies it, and re-writes it. Hence you may test HtmlConv using any file that contains a text/html email. 

Warning: HtmlConv overwrites the original email file. So, you want to make a backup of the original file before testing HtmlConv.

However, make shure that the input email file also contains the header of the email. If you configure PMMail not to display email headers by default, it will also not save the header to a file. To save the body and the header in this case, you need to open the email, click on "Toggle Display Of The Message Header" and then save it to a file.

----------------------------------------------------------------------------
The author
----------------------------------------------------------------------------

Enter "htmlconv /h" to get my name and my email address.

You may request my PGP public key by sending me an empty email with "send pgp key" in the subject line.

----------------------------------------------------------------------------
Miscellaneous
----------------------------------------------------------------------------

If you know my program RichConv, which converts text/enriched emails, you may have noticed that this file is a recycled form of the RichConv ReadMe file ;-)

If you don't know RichConv you may want to check it out. Should be available on the same server as HtmlConv.







--
$Id: readme,v 1.5 1998-10-10 11:40:39-04 rl Exp $
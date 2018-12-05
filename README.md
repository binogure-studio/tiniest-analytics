# google-analytics

Google Analytics is a *very* simple to use, cross-platform (tested on win/osx/linux) and basically *very tiny* analytics system written in C++ (less than 150 lines of code), made specifically for games. It uses libcurl to post events to your Google Analytics account.

## Usage

Call `GoogleAnalytics.initialize("UA-12345678-1", uniqueId)` at startup, and `GoogleAnalytics.flush()` each frame. You also need libcurl in your project (ssl not needed).

There's only one function to use when posting an event, e.g.

```
GoogleAnalytics.send_event_action("GameStart", "linux");
GoogleAnalytics.send_event_action_label("WeaponClicked", "RocketLauncher", "Loading);
GoogleAnalytics.send_event_action_label_value("MapStarted", "SinglePlayer", "MapName.map", 150);
```

![alt text](https://user-images.githubusercontent.com/292486/33986723-206b0bc8-e0c7-11e7-9ac9-8f4f841385ed.jpg)


![alt text](https://user-images.githubusercontent.com/292486/33986724-20895b32-e0c7-11e7-8114-b203803867e4.jpg)


![alt text](https://user-images.githubusercontent.com/292486/33986722-204d395e-e0c7-11e7-8d1d-4f73cfcb577a.jpg)

Original authors:
Mihai Dragomir - dmc@pixelshard.com
Mihai Gosa (Pintea) - pintea@inthekillhouse.com or @gosamihai on twitter
Initially developed for the 'Door Kickers' games.

Forked by:
Xavier Sellier - contact@binogure-studio.com
Enhanced for City Game Studio game

License: MIT

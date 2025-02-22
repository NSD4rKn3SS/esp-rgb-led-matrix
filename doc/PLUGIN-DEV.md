# PIXELIX
![PIXELIX](./images/LogoBlack.png)

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](http://choosealicense.com/licenses/mit/)

- [PIXELIX](#pixelix)
- [Plugin development](#plugin-development)
  - [What must be done?](#what-must-be-done)
  - [Rules](#rules)
  - [Recommendations](#recommendations)
  - [Typical use cases](#typical-use-cases)
    - [Initial configuration in fileystem](#initial-configuration-in-fileystem)
    - [Reload configuration from filesystem periodically](#reload-configuration-from-filesystem-periodically)
    - [Request information from URL periodically](#request-information-from-url-periodically)
  - [Traps and pitfalls](#traps-and-pitfalls)
    - [active/inactive](#activeinactive)

# Plugin development

## What must be done?

1. The plugin sources (.h/.cpp) shall be in ```/src/Plugin/Plugins``` folder.
2. The plugin shall be registered in the module ```/src/StateMachine/InitState.cpp```, function ```void InitState::registerPlugins()```.
3. A short plugin description shall be in ```/doc/PLUGINS.md```.
4. A plugin specific HTML page shall be in ```/data/plugins```. Add the page to the menu in ```/data/js/menu.js```.
5. Place a plugin screenshot as image in ```/data/images```, using the plugin name and the file extension. It shall be shown in the HTML page.
6. If the plugin spawns a REST API:
   1. The HTML page must be able to get/set the corresponding information via REST API.
   2. The [REST API description](https://app.swaggerhub.com/apis/BlueAndi/Pixelix) shall be extended. Ask the owner to do this via issue or pull-request.

## Rules
* Follow the [boy scout rule](https://biratkirat.medium.com/step-8-the-boy-scout-rule-robert-c-martin-uncle-bob-9ac839778385#:~:text=The%20Boy%20Scouts%20have%20a,not%20add%20more%20smelling%20code.), especially for coding style. Check other plugins or sourcecode in the repository, to know how to do it right.
* All one time jobs shall be done in ```start()``` method. E.g. the initial layout creation.
* The ```update()``` method shall be efficient, therefore
  * never read/write something from filesystem,
  * never perform any web related stuff,
  * never do one time jobs inside,
  * just update the display!

## Recommendations
* Update the display only, if the content changed.

## Typical use cases

### Initial configuration in fileystem
The first time a plugin instance starts up, it will try to load a configuration from the filesystem (if applicable) in ```start()``` method. If this fails, it creates a default one.

### Reload configuration from filesystem periodically
Because a plugin instance configuration in the filesystem can be edited via file browser too, it makes sense to periodically reload it. It is recommended to do this in the ```process()``` method.

### Request information from URL periodically
Any http request can be started in the ```process()``` method. The response will be evaluated in the context of the corresponding web task. Only the take over of the relevant data shall be protected against concurrent access.

## Traps and pitfalls

### active/inactive
The ```active()``` and ```inactive()``` methods are called once before a plugin instance is activated or deactivated. But consider the case, that only one plugin instance is installed at all (except SysMsgPlugin). ```active()``` will be called just once and stays active.

Therefore if you need periodically stuff, but you can't do it in the ```update()``` method, use the ```process()``` method.


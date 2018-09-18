Lightbox Campaigns
==================

CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Recommended Modules
 * Configuration
 * Use
 * Maintainers
 * License

INTRODUCTION
------------

Lightbox Campaigns enables the creation of "campaigns" that can be configured to
display full page content to users based roles, content types, and/or paths.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/lightbox_campaigns
   
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/lightbox_campaigns

REQUIREMENTS
------------

This module requires the following modules:

 * [Entity API](https://drupal.org/project/entity)
 * [Date API](https://drupal.org/project/date)
 * [jQuery Update](https://drupal.org/project/jquery_update)
 * [Libraries](https://drupal.org/project/libraries)

This module requires the following external libraries:

 * [Featherlight.js](https://noelboss.github.io/featherlight/)
 
   *Note: Featherlight.js requires jQuery version 1.7 or higher. This is why
   jQuery Update is a dependency for Lightbox Campaigns.*

RECOMMENDED MODULES
-------------------

 * [Rules](https://www.drupal.org/project/rules):
   When enabled, Lightbox Campaigns provides a "Display a campaign" action
   that can triggered by Rules events.

INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. Visit:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.
   
 * Ensure the site jQuery version is at least 1.7 (Administration » 
   Configuration » Development).
   
 * Install the Featherlight.js library:
 
   - Download the latest release from 
     https://github.com/noelboss/featherlight/releases.
     
   - Unpack the library files.
   
   - Place the files in your site's `sites/all/libraries` folder such that the
     location is `sites/all/libraries/featherlight`.
     
   Installation can be verified from Reports » Status Report. There should be a 
   message similar to: "Featherlight `VERSION` installed at `LOCATION`."

CONFIGURATION
-------------

 * Configure user permissions in Administration » People » Permissions:
   
   - Administer Lightbox Campaigns
   
     Users need this permission to access and manage campaigns.

USE
---

Lightbox Campaign entities can be added from Content » Lightbox Campaigns.

Every campaign entity has the following fields:

 * Title (string)
 
   The name used in administrative pages to identify a campaign.
 
 * Enabled (boolean)
 
   Whether or not the campaign is enabled. Campaigns that are not enabled will 
   not display to users under any circumstances.
   
 * Lightbox content (formatted text)
 
   The content to appear in the lightbox displayed to users.
   
 * Reset timer (select)
 
   The amount of time to wait before displaying the lightbox to a user who has
   already seen it.
   
 * Start date/time (datetime)
 
   Earliest date and time that the lightbox should begin displaying to users.
   
 * End date/time (datetime)
 
   Latest date and time that the lightbox should display to users.
   
 * Visibility
 
   The following rules can be used to fine-grain the circumstances under which
   the campaigns's lightbox will be displayed to users. Any number of rules can
   be combined and *all* rules must pass for content to be displayed.
   
    - Pages
  
     If paths are listed and the "Only the listed pages" option is selected,
     the campaign will only appear on the listed paths.
    
     If the "All pages except those listed" option is selected, the campaign
     will display on any path *expect* the listed paths.
    
     If no paths are listed, the campaign will display on *any* path.
 
   - Content Types
   
     If any content types are selected, the campaign will only display on pages 
     of the content types selected.
     
     If no content types are selected, the campaign will display on pages of 
     *any* content type.
     
   - Roles
      
     If any user roles are selected, the campaign will only to users belonging
     to the selected roles.
     
     If no user roles are selected, the campaign will display to *all* users.
      
MAINTAINERS
-----------

Current maintainers:
 * Christopher Charbonneau Wells (wells) - https://www.drupal.org/u/wells

This project is sponsored by:
 * [Cascade Public Media](https://www.drupal.org/cascade-public-media) for 
 [KCTS9.org](https://kcts9.org/) and [Crosscut.com](https://crosscut.com/).
 
LICENSE
-------

All code in this repository is licensed 
[GPLv2](http://www.gnu.org/licenses/gpl-2.0.html). A LICENSE file is not 
included in this repository per Drupal's module packaging specifications.

See [Licensing on Drupal.org](https://www.drupal.org/about/licensing).

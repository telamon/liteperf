# ext/faye_websocket/extconf.rb

require 'mkmf'
extension_name = 'tgfx'
dir_config(extension_name)
create_makefile(extension_name)

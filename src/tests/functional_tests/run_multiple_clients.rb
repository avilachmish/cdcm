#!/usr/bin/env ruby
folder = File.expand_path('.',__dir__)
$:.unshift(folder) unless $:.include?(folder)

#require 'mdclient2.rb'

fork { exec("ruby mdclient2.rb actions1.xml 1") }
fork { exec("ruby mdclient2.rb actions2.xml 2") }

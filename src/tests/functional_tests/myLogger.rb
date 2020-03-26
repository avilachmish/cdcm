#!/usr/bin/env ruby
require 'fileutils'
require 'json'
# https://ruby-doc.org/stdlib-2.6.3/libdoc/logger/rdoc/Logger.html
# TODO: write destructor that close the log file: @logger.close https://stackoverflow.com/questions/5956067/ruby-destructors
class MyLogger
    include Singleton
    def initialize(log_location='./log/')
        settings_file = File.open "./testsRunner_settings.json"
        #log_file = File.open('cdcm_client.log', File::WRONLY |  File::CREAT | File::APPEND )
	    settings_data = JSON.load settings_file
	    log_file = File.open("/var/log/cdcm/testsRunner/cdcm_client#{$client_id}.log", File::WRONLY | File::CREAT | File::TRUNC)
	    settings_file.close
	    logs_dir = settings_data["logger"]["logs_dir"].to_s
	    response = FileUtils.mkdir_p(logs_dir)
	    log_file = File.open("#{logs_dir}/cdcm_client#{$client_id}.log", File::WRONLY | File::CREAT | File::TRUNC)log_file.sync = true
        @logger = Logger.new( log_file )
        @logger.datetime_format = '%d-%m-%Y %H:%M:%S::%3N'
        @logger.formatter = proc do |severity, datetime, progname, msg|
        "[#{severity}] [#{datetime.strftime(@logger.datetime_format)}] [#{progname}]: #{msg}\n"
    end

    def log
        @logger
    end

end #end of class MyLogger

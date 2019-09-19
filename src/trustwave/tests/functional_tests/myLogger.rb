#!/usr/bin/env ruby

# https://ruby-doc.org/stdlib-2.6.3/libdoc/logger/rdoc/Logger.html
# TODO: write destructor that close the log file: @logger.close https://stackoverflow.com/questions/5956067/ruby-destructors
class MyLogger
    include Singleton
    def initialize
        log_file = File.open('/var/cdcm/log/cdcm_client.log', File::WRONLY |  File::CREAT | File::APPEND )
        #log_file = File.open('/var/cdcm/log/cdcm_client.log', File::WRONLY | File::CREAT | File::TRUNC)
        log_file.sync = true
        @logger = Logger.new( log_file )
        @logger.datetime_format = '%d--%m-%Y %H:%M:%S '
        @logger.formatter = proc do |severity, datetime, progname, msg|
            "[#{severity}] [#{datetime}] [#{progname}:#{__LINE__}]: #{msg}\n"

        end
    end

    def log
        @logger
    end

end #end of class MyLogger

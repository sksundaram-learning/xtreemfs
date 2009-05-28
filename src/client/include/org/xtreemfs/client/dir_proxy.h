// Copyright 2009 Minor Gordon.
// This source comes from the XtreemFS project. It is licensed under the GPLv2 (see COPYING for terms and conditions).

#ifndef _ORG_XTREEMFS_CLIENT_DIR_PROXY_H_
#define _ORG_XTREEMFS_CLIENT_DIR_PROXY_H_

#include "org/xtreemfs/client/proxy.h"

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4100 )
#endif
#include "org/xtreemfs/interfaces/dir_interface.h"
#ifdef _WIN32
#pragma warning( pop )
#endif

#include <map>
#include <string>


namespace org
{
  namespace xtreemfs
  {
    namespace client
    {
      class DIRProxy : public Proxy<DIRProxy, org::xtreemfs::interfaces::DIRInterface>
      {
      public:
        template <class StageGroupType>
        static YIELD::auto_Object<DIRProxy> create( const YIELD::URI& absolute_uri,
                                                    YIELD::auto_Object<StageGroupType> stage_group,
                                                    YIELD::auto_Object<YIELD::Log> log = NULL,
                                                    const YIELD::Time& operation_timeout = YIELD::ONCRPCClient<org::xtreemfs::interfaces::DIRInterface>::OPERATION_TIMEOUT_DEFAULT,
                                                    uint8_t reconnect_tries_max = YIELD::ONCRPCClient<org::xtreemfs::interfaces::DIRInterface>::RECONNECT_TRIES_MAX_DEFAULT
#ifdef YIELD_HAVE_OPENSSL
                                                    , YIELD::auto_Object<YIELD::SSLContext> ssl_context = NULL
#endif
                                                  )
        {
          return YIELD::ONCRPCClient<org::xtreemfs::interfaces::DIRInterface>::create<DIRProxy>( absolute_uri, stage_group, log, operation_timeout, reconnect_tries_max
#ifdef YIELD_HAVE_OPENSSL
                                                                                  , ssl_context
#endif
                                                                                  );
        }

        YIELD::auto_Object<YIELD::URI> getURIFromUUID( const std::string& uuid );
        YIELD::auto_Object<YIELD::URI> getVolumeURIFromVolumeName( const std::string& volume_name );

      private:
        friend class YIELD::ONCRPCClient<org::xtreemfs::interfaces::DIRInterface>;

        DIRProxy( YIELD::auto_Object<YIELD::FDAndInternalEventQueue> fd_event_queue, YIELD::auto_Object<YIELD::Log> log, const YIELD::Time& operation_timeout, YIELD::auto_Object<YIELD::SocketAddress> peer_sockaddr, uint8_t reconnect_tries_max, YIELD::auto_Object<YIELD::Socket> _socket );
        ~DIRProxy();


        class CachedAddressMappingURI : public YIELD::URI
        {
        public:
          CachedAddressMappingURI( const std::string& uri, uint32_t ttl_s )
            : YIELD::URI( uri ), ttl_s( ttl_s )
          {
            creation_epoch_time_s = YIELD::Time::getCurrentUnixTimeS();
          }

          uint32_t get_ttl_s() const { return ttl_s; }
          double get_creation_epoch_time_s() const { return creation_epoch_time_s; }

          // Object
          CachedAddressMappingURI& incRef() { return YIELD::Object::incRef( *this ); }

        private:
          ~CachedAddressMappingURI() { }

          uint32_t ttl_s;
          double creation_epoch_time_s;
        };

        std::map<std::string, CachedAddressMappingURI*> uuid_to_uri_cache;
        YIELD::Mutex uuid_to_uri_cache_lock;
      };
    };
  };
};

#endif

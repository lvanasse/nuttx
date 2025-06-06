/****************************************************************************
 * net/neighbor/neighbor.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __NET_NEIGHBOR_NEIGHBOR_H
#define __NET_NEIGHBOR_NEIGHBOR_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

#include <net/ethernet.h>

#include <nuttx/net/ip.h>
#include <nuttx/net/netdev.h>
#include <nuttx/net/sixlowpan.h>
#include <nuttx/net/neighbor.h>

#ifdef CONFIG_NET_IPv6

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* This is the Neighbor table.  The network should be locked when accessing
 * this table.
 */

extern struct neighbor_entry_s g_neighbors[CONFIG_NET_IPv6_NCONF_ENTRIES];

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

struct net_driver_s; /* Forward reference */

/****************************************************************************
 * Name: neighbor_findentry
 *
 * Description:
 *   Find an entry in the Neighbor Table.  This interface is internal to
 *   the neighbor implementation; Consider using neighbor_lookup() instead;
 *
 * Input Parameters:
 *   ipaddr - The IPv6 address to use in the lookup;
 *
 * Returned Value:
 *   The Neighbor Table entry corresponding to the IPv6 address;  NULL is
 *   returned if there is no matching entry in the Neighbor Table.
 *
 ****************************************************************************/

FAR struct neighbor_entry_s *neighbor_findentry(const net_ipv6addr_t ipaddr);

/****************************************************************************
 * Name: neighbor_add
 *
 * Description:
 *   Add the new address association to the Neighbor Table (if it is not
 *   already there).
 *
 * Input Parameters:
 *   dev    - Driver instance associated with the MAC
 *   ipaddr - The IPv6 address of the mapping.
 *   addr   - The link layer address of the mapping
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void neighbor_add(FAR struct net_driver_s *dev, FAR net_ipv6addr_t ipaddr,
                  FAR uint8_t *addr);

/****************************************************************************
 * Name:  neighbor_lookup
 *
 * Description:
 *   Find an entry in the Neighbor Table and return its link layer address.
 *
 * Input Parameters:
 *   ipaddr - The IPv6 address to use in the lookup;
 *   laddr  - Location to return the corresponding link layer address.
 *            This address may be NULL.  In that case, this function may be
 *            used simply to determine if the link layer address is
 *            available.
 *
 * Returned Value:
 *   Zero (OK) if the link layer address is returned.  A negated errno value
 *   is returned on any error.
 *
 ****************************************************************************/

int neighbor_lookup(FAR const net_ipv6addr_t ipaddr,
                    FAR struct neighbor_addr_s *laddr);

/****************************************************************************
 * Name: neighbor_update
 *
 * Description:
 *   Reset time on the Neighbor Table entry associated with the IPv6 address.
 *   This makes the associated entry the most recently used and not a
 *   candidate for removal.
 *
 * Input Parameters:
 *   ipaddr - The IPv6 address of the entry to be updated
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void neighbor_update(const net_ipv6addr_t ipaddr);

/****************************************************************************
 * Name: neighbor_ethernet_out
 *
 * Description:
 *   This function should be called before sending out an IPv6 packet. The
 *   function checks the destination IPv6 address of the IPv6 packet to see
 *   what Ethernet MAC address that should be used as a destination MAC
 *   address on the Ethernet.
 *
 *   If the destination IPv6 address is in the local network (determined
 *   by logical ANDing of netmask and our IPv6 address), the function
 *   checks the Neighbor Table to see if an entry for the destination IPv6
 *   address is found.  If so, an Ethernet header is prepended at the
 *   beginning of the packet and the function returns.
 *
 *   If no Neighbor Table entry is found for the destination IPv6 address,
 *   the packet in the d_buf is replaced by an ICMPv6 Neighbor Solicit
 *   request packet for the IPv6 address. The IPv6 packet is dropped and
 *   it is assumed that the higher level protocols (e.g., TCP) eventually
 *   will retransmit the dropped packet.
 *
 *   Upon return in either the case, a packet to be sent is present in the
 *   d_buf buffer and the d_len field holds the length of the Ethernet
 *   frame that should be transmitted.
 *
 ****************************************************************************/

#ifdef CONFIG_NET_ETHERNET
void neighbor_ethernet_out(FAR struct net_driver_s *dev);
#endif

/****************************************************************************
 * Name: neighbor_snapshot
 *
 * Description:
 *   Take a snapshot of the current state of the Neighbor table.
 *
 * Input Parameters:
 *   snapshot  - Location to return the Neighbor table copy
 *   nentries  - The size of the user provided 'dest' in entries, each of
 *               size sizeof(struct neighbor_entry_s)
 *
 * Returned Value:
 *   On success, the number of entries actually copied is returned.  Unused
 *   entries are not returned.
 *
 * Assumptions
 *   The network is locked to assure exclusive access to the ARP table
 *
 ****************************************************************************/

#ifdef CONFIG_NETLINK_ROUTE
unsigned int neighbor_snapshot(FAR struct neighbor_entry_s *snapshot,
                               unsigned int nentries);
#endif

/****************************************************************************
 * Name: neighbor_dumpentry
 *
 * Description:
 *   Dump the contents of an entry Neighbor Table.
 *
 * Input Parameters:
 *   msg      - Message to print with the entry
 *   neighbor - The table entry to dump
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_NET_INFO
void neighbor_dumpentry(FAR const char *msg,
                        FAR struct neighbor_entry_s *neighbor);
#else
#  define neighbor_dumpentry(msg,neighbor)
#endif

/****************************************************************************
 * Name: neighbor_dumpipaddr
 *
 * Description:
 *   Dump an IP address.
 *
 * Input Parameters:
 *   msg    - Message to print with the entry
 *   ipaddr - The IP address to dump
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_NET_INFO
void neighbor_dumpipaddr(FAR const char *msg,
                         const net_ipv6addr_t ipaddr);
#else
#  define neighbor_dumpipaddr(msg,ipaddr)
#endif

#endif /* CONFIG_NET_IPv6 */
#endif /* __NET_NEIGHBOR_NEIGHBOR_H */

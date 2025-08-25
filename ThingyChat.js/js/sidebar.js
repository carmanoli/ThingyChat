document.addEventListener("DOMContentLoaded", function () {
  const mainContent = document.getElementById("main-content");
  const navLinks = document.querySelectorAll("#sidebar .nav-link");
  const sidebar = document.getElementById("sidebar");

  // Função para carregar o CSS de uma página de forma dinâmica
  function loadPageStyles(cssUrl) {
    const oldLink = document.querySelector("link[data-page-style]");
    if (oldLink) {
      oldLink.remove();
    }
    if (!cssUrl) return;
    if (document.querySelector(`link[href="${cssUrl}"]`)) {
      return;
    }
    const link = document.createElement("link");
    link.rel = "stylesheet";
    link.href = cssUrl;
    link.setAttribute("data-page-style", "true");
    document.head.appendChild(link);
  }

  // Função para carregar o conteúdo e executar scripts
  function executeScripts(container) {
    const scripts = container.querySelectorAll("script");
    scripts.forEach((oldScript) => {
      const newScript = document.createElement("script");
      Array.from(oldScript.attributes).forEach((attr) =>
        newScript.setAttribute(attr.name, attr.value)
      );
      newScript.textContent = oldScript.textContent;
      oldScript.parentNode.replaceChild(newScript, oldScript);
    });
  }

  // Função principal para carregar o conteúdo de uma página
  async function loadContent(url) {
    try {
      const response = await fetch(url);
      if (!response.ok)
        throw new Error(`HTTP error! status: ${response.status}`);
      const html = await response.text();
      const parser = new DOMParser();
      const doc = parser.parseFromString(html, "text/html");
      const pageStyleLink = doc.querySelector('link[rel="stylesheet"]');
      const cssUrl = pageStyleLink ? pageStyleLink.getAttribute("href") : null;
      loadPageStyles(cssUrl);
      mainContent.innerHTML = doc.body.innerHTML;
      executeScripts(mainContent);
    } catch (error) {
      mainContent.innerHTML = `<div class="alert alert-danger">Error loading page: ${error.message}</div>`;
      console.error("Failed to fetch page:", error);
    }
  }

  // Função para atualizar a lista de dispositivos na sidebar
  function updateDeviceList() {
    const knownDevices = [
      { name: "emoji16_display.local", ip: "10.0.1.174" },
      { name: "memento_mori.local", ip: "10.0.1.175" },
    ];
    const deviceList = document.createElement("ul");
    deviceList.className = "nav nav-pills flex-column mt-3";
    deviceList.id = "device-list";

    knownDevices.forEach((device) => {
      const li = document.createElement("li");
      li.className = "nav-item";
      const a = document.createElement("a");
      a.className = "nav-link";
      a.href = `pages/${device.name.split(".")[0]}.html?device=${device.name}`;
      a.textContent = device.name;
      // Try pinging the device to check if it's online
      fetch(`http://${device.name}/`, { method: "HEAD", mode: "no-cors" })
        .then(() => {
          a.style.color = "green"; // Online
        })
        .catch(() => {
          a.style.color = "white"; // Offline
          // Fallback to IP if mDNS fails
          a.href = `pages/${device.name.split(".")[0]}.html?device=${
            device.ip
          }`;
        });
      a.addEventListener("click", (e) => {
        e.preventDefault();
        loadContent(a.href);
        navLinks.forEach((l) => l.classList.remove("active"));
        document
          .querySelectorAll("#device-list .nav-link")
          .forEach((l) => l.classList.remove("active"));
        a.classList.add("active");
        if (window.innerWidth <= 991.98) {
          sidebar.classList.remove("active");
        }
      });
      li.appendChild(a);
      deviceList.appendChild(li);
    });

    const existingList = sidebar.querySelector("#device-list");
    if (existingList) existingList.remove();
    sidebar.appendChild(deviceList);
  }

  // Lógica para os links da sidebar
  navLinks.forEach((link) => {
    link.addEventListener("click", function (event) {
      event.preventDefault();
      const url = this.getAttribute("href");
      loadContent(url);
      navLinks.forEach((l) => l.classList.remove("active"));
      document
        .querySelectorAll("#device-list .nav-link")
        .forEach((l) => l.classList.remove("active"));
      this.classList.add("active");
      if (window.innerWidth <= 991.98) {
        sidebar.classList.remove("active");
      }
    });
  });

  // Lógica do Burger Menu
  const sidebarToggle = document.getElementById("sidebarToggle");
  if (sidebarToggle) {
    sidebarToggle.addEventListener("click", (e) => {
      e.stopPropagation();
      sidebar.classList.toggle("active");
    });
  }
  document.addEventListener("click", (event) => {
    const isMobile = window.innerWidth <= 991.98;
    if (
      isMobile &&
      sidebar.classList.contains("active") &&
      !sidebar.contains(event.target) &&
      !sidebarToggle.contains(event.target)
    ) {
      sidebar.classList.remove("active");
    }
  });

  // Carrega o conteúdo inicial
  const initialActiveLink = document.querySelector("#sidebar .nav-link.active");
  if (initialActiveLink) {
    loadContent(initialActiveLink.getAttribute("href"));
  }

  // Carrega e atualiza a lista de dispositivos
  updateDeviceList();
  setInterval(updateDeviceList, 30000); // Atualiza a cada 30 segundos

  // Lida com eventos de navegação (para compatibilidade com WebSocket)
  window.addEventListener("navigate", function (event) {
    const url = event.detail.url;
    console.log(`Navegação automática acionada para: ${url}`);
    loadContent(url);
  });

    // --- LÓGICA DE NOTIFICAÇÃO DE FOTO ---
    const editorLink = document.querySelector('a[href="pages/editor.html"]');

    // Ouve pelo evento que o serviço dispara quando uma foto está pronta
    window.addEventListener('photoready', () => {
        if (editorLink && !editorLink.querySelector('.photo-badge')) {
            editorLink.innerHTML += ' <span class="badge bg-warning photo-badge">📷</span>';
        }
    });

    // Ouve pelo evento que o editor irá disparar depois de a foto ser "colada"
    window.addEventListener('photopasted', () => {
        const badge = editorLink ? editorLink.querySelector('.photo-badge') : null;
        if (badge) {
            badge.remove();
        }
    });

    
});

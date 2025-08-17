document.addEventListener('DOMContentLoaded', function() {
    const mainContent = document.getElementById('main-content');
    const navLinks = document.querySelectorAll('#sidebar .nav-link');

    // Função para carregar o CSS de uma página de forma dinâmica
    function loadPageStyles(cssUrl) {
        // 1. Limpa o CSS antigo da página anterior para evitar conflitos
        const oldLink = document.querySelector('link[data-page-style]');
        if (oldLink) {
            oldLink.remove();
        }

        // 2. Se não houver CSS novo, não faz nada
        if (!cssUrl) return;

        // 3. Verifica se o CSS já não está carregado
        if (document.querySelector(`link[href="${cssUrl}"]`)) {
            return;
        }

        // 4. Cria e adiciona o novo link de CSS ao <head>
        const link = document.createElement('link');
        link.rel = 'stylesheet';
        link.href = cssUrl;
        link.setAttribute('data-page-style', 'true'); // Marca-o para ser removido depois
        document.head.appendChild(link);
    }

    // Função para carregar o conteúdo e executar scripts
    function executeScripts(container) {
        const scripts = container.querySelectorAll('script');
        scripts.forEach(oldScript => {
            const newScript = document.createElement('script');
            Array.from(oldScript.attributes).forEach(attr => newScript.setAttribute(attr.name, attr.value));
            newScript.textContent = oldScript.textContent;
            oldScript.parentNode.replaceChild(newScript, oldScript);
        });
    }

    // Função principal para carregar o conteúdo de uma página
    async function loadContent(url) {
        try {
            const response = await fetch(url);
            if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
            
            const html = await response.text();
            
            // Usamos um truque para analisar o HTML sem o adicionar à página ainda
            const parser = new DOMParser();
            const doc = parser.parseFromString(html, 'text/html');

            // Procura por um link de CSS dentro do conteúdo carregado
            const pageStyleLink = doc.querySelector('link[rel="stylesheet"]');
            const cssUrl = pageStyleLink ? pageStyleLink.getAttribute('href') : null;

            // Carrega o CSS
            loadPageStyles(cssUrl);

            // Coloca o conteúdo (sem o link de CSS) no main
            mainContent.innerHTML = doc.body.innerHTML;
            
            // Executa os scripts
            executeScripts(mainContent);

        } catch (error) {
            mainContent.innerHTML = `<div class="alert alert-danger">Error loading page: ${error.message}</div>`;
            console.error('Failed to fetch page:', error);
        }
    }

    // Lógica para os links da sidebar
    navLinks.forEach(link => {
        link.addEventListener('click', function(event) {
            event.preventDefault();
            const url = this.getAttribute('href');
            loadContent(url);
            navLinks.forEach(l => l.classList.remove('active'));
            this.classList.add('active');
            if (window.innerWidth <= 991.98) {
                document.getElementById('sidebar').classList.remove('active');
            }
        });
    });

    // --- Lógica do Burger Menu (sem alterações) ---
    const sidebar = document.getElementById('sidebar');
    const sidebarToggle = document.getElementById('sidebarToggle');
    if (sidebarToggle) {
        sidebarToggle.addEventListener('click', e => {
            e.stopPropagation();
            sidebar.classList.toggle('active');
        });
    }
    document.addEventListener('click', event => {
        const isMobile = window.innerWidth <= 991.98;
        if (isMobile && sidebar.classList.contains('active') && !sidebar.contains(event.target) && !sidebarToggle.contains(event.target)) {
            sidebar.classList.remove('active');
        }
    });

    // Carrega o conteúdo inicial
    const initialActiveLink = document.querySelector('#sidebar .nav-link.active');
    if (initialActiveLink) {
        loadContent(initialActiveLink.getAttribute('href'));
    }

        window.addEventListener('navigate', function(event) {
        const url = event.detail.url;
        console.log(`Navegação automática acionada para: ${url}`);
        
        // Reutiliza a nossa função de carregar conteúdo
        loadContent(url);
    });
});